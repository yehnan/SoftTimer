#include "SoftTimer.h"

void pf(const char *fmt, ... ){
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    Serial.print(tmp);
}

SoftTimer::SoftTimer(void)
{
    for(int8_t i = 0; i < EVENTS_MAX; i++){
        events[i] = NULL;
    }
}

int8_t SoftTimer::findEventIndex(EventBase* evt)
{
    for(int8_t i = 0; i < EVENTS_MAX; i++){
        if(events[i] == evt){
            return i;
        }
    }
    return EVENTS_MAX;
}

int16_t SoftTimer::addEvent(EventBase* evt)
{
    const int8_t i = findFreeEventIndex();
    if(isIndexValid(i)){
        if(evt->nextTriggerTime == 0){
            evt->nextTriggerTime = millis();
        }
        events[i] = evt;
        
        return events[i]->id;
    }
    
    return INVALID_EVENT_ID;
}

boolean SoftTimer::removeEventByIndex(int8_t i)
{
    if(isIndexValid(i)){
        if(events[i]->deleteWhenRemove){
            delete events[i];
        }
        events[i] = NULL;
        return true;
    }
    return false;
}

int16_t SoftTimer::removeAllEvents(void)
{
    int16_t cnt = 0;
    
    for(int8_t i = 0; i < EVENTS_MAX; i++){
        cnt += removeEventByIndex(i) == true ? 1 : 0;
    }
    
    return cnt;
}

// the workhorse of this library
boolean SoftTimer::update(uint8_t i)
{
    boolean triggered = false;
    
    // trigger or not, by checking time
    if(isIndexValid(i) && millis() >= events[i]->nextTriggerTime){ 
        triggered = true;

        if((events[i]->callback)(events[i])){ // execute callback, pass in the event itself
            removeEventByIndex(i); // if callback returns true, remove the event.
        }
        else{
            // repeatCount: positive, 0(needed to remove), negative(forever)
            events[i]->repeatCount -= events[i]->repeatCount > 0 ? 1 : 0;
            
            if(events[i]->repeatCount == 0){ // 0, remove it
                removeEventByIndex(i);
            }
            else{ // not 0 (positive or negative), calculate the next trigger time
                events[i]->nextTriggerTime += events[i]->period;
            }
        }
    }
    
    return triggered;
}
int16_t SoftTimer::update(void)
{
    int16_t cnt = 0; // count of events triggered
    
    for(int8_t i = 0; i < EVENTS_MAX; i++){ // iterate all possible events
        if(events[i] != NULL){ // ok, find a valid event (i.e. not NULL)
            cnt += update(i) == true ? 1 : 0;
        }
    }
    
    return cnt;
}

int16_t SoftTimer::schedule(uint32_t period, CB_TYPE callback, int16_t repeatCount, int32_t after)
{
    int16_t id = INVALID_EVENT_ID;
    const int8_t i = findFreeEventIndex();
    if(isIndexValid(i)){
        EventBase* evt = new EventBase();
        
        evt->period = period;
        evt->callback = callback;
        evt->repeatCount = repeatCount;
        evt->deleteWhenRemove = true;
        
        evt->nextTriggerTime = millis() + (after >= 0 ? after : period);

        return addEvent(evt);
    }
    
    return id;
}

static boolean callback_oscillate(EventBase* evt)
{
    EventPinState* e = (EventPinState*) evt;
    digitalWrite(e->pin, e->state);
    e->state = !e->state;
    
    return false;
}

int16_t SoftTimer::oscillate(uint8_t pin, uint8_t startingState, uint32_t period, int16_t repeatCount, int32_t after)
{
    int16_t id = INVALID_EVENT_ID;
    const int8_t i = findFreeEventIndex();
    if(isIndexValid(i)){
        EventPinState* evt = new EventPinState();
        
        evt->period = period;
        evt->callback = &callback_oscillate;
        evt->repeatCount = repeatCount * 2; //!!! full cycles, not transitions
        evt->deleteWhenRemove = true;
        
        evt->nextTriggerTime = millis() + (after >= 0 ? after : period);
        
        evt->pin = pin;
        evt->state = startingState;

        return addEvent(evt);
    }
    
    return id;
}

