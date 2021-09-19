#include "SoftTimer.h"

SoftTimer timer;

int idLedEvent;

void setup()
{
    Serial.begin(115200);

    int idTickEvent = timer.every(2000, callbackTick);
    Serial.print("2 second tick event started, id=");
    Serial.println(idTickEvent);

    pinMode(13, OUTPUT);
    idLedEvent = timer.oscillate(13, HIGH, 1000);
    Serial.print("LED event started, id=");
    Serial.println(idLedEvent);

    int idOnceEvent = timer.once(callbackOnce, 5000);
    Serial.print("once event started, id=");
    Serial.println(idOnceEvent); 
}

void loop()
{
    timer.update();
}

bool callbackTick(EventBase* evt)
{
    const unsigned long now = millis();
    Serial.print("2 second tick event: millis()=");
    Serial.println(now);

    // although we use 'every' to set the 2 second tick event,
    // but still can utilize the return value of the callback 
    // to denote whether to remove this event or not
    if(now <= 20000){
        return false; // not remove
    }
    else{
        Serial.print("2 second tick event be removed.");
        return true; // remove it
    }
}


bool callbackOnce(EventBase* evt)
{
    Serial.println("In the callback function of once event.");
    Serial.println("Stop the original led event(slow blink).");
    timer.removeEvent(idLedEvent);
    
    Serial.println("Start new led event(fast blink).");
    timer.oscillate(13, HIGH, 300);

    return false;
}
