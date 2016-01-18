#ifndef __Soft_Timer_h__
#define __Soft_Timer_h__

// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>
#include "EventBase.h"

// EVENTS_MAX defines the max count of events of a Timer object.
// Valid index of event array 'events'  is from 0 to (EVENTS_MAX - 1),
// EVENTS_MAX is also used as invalid event index internally.
// EVENTS_MAX should be only used in internal implementation.
// Users of this library can change this macro constant, but should not use it.
#define EVENTS_MAX (10)

// This library uses the object's address as the event id.
// Addresses should be positive, so use negative value(-1) to denote invalid id.
// Users can/should use this constant to denote invalid event id.
#define INVALID_EVENT_ID (-1)

class SoftTimer
{
public:
    SoftTimer(void);
    
    // Let the timer object do its job. Sould be in loop().
    // return the count of events be triggered
    int16_t update(void);

    // argument 'period': the time between two 'callback' be executed(triggered).
    
    // argument 'callback': callback function,
    // prototype is CB_TYPE, which is boolean (*)(EventBase*)
    // note: callback will get the event as argument.
    // if callback returns true, timer object will remove the event;
    // if returns false, just continue...
    // canonical implementation is:
    // boolean callbackOfSomeEvent(EventBase* evt)
    // {
    //     return false; // !don't forget! or the C language's behavior is undefined
    // }
    
    // argument 'after': if >= 0, the first trigger time is now + 'after';
    // if negative, the first trigger time is now + 'period';
    
    // argument 'repeatCount': negative(like -1): forever, 0: will be removed, 
    
    // return event id; if faiiled, return INVALID_EVENT_ID
    int16_t schedule(uint32_t period, CB_TYPE callback, int16_t repeatCount=-1, int32_t after=0);
    
    // execute 'callback' every 'period' time; the first trigger time is after 'after' time
    int16_t every(uint32_t period, CB_TYPE callback, int32_t after=0){
        return schedule(period, callback, -1, after);
    };
    
    // execute 'callback' only once, after 'after' time
    int16_t once(CB_TYPE callback, int32_t after){
        return schedule(0, callback, 1, after);
    };

    // change the state of the output pin to startingState and !startingState periodically
    // note: one repeatCount means two state changes
    int16_t oscillate(uint8_t pin, uint8_t startingState, uint32_t period, int16_t repeatCount=-1, int32_t after=0);
    
    // just a convenient wrapper around oscillate
    int16_t ledBlink(uint8_t pin, uint8_t startingState, uint32_t period, int16_t repeatCount=-1, int32_t after=0){
        return oscillate(pin, startingState, period, repeatCount, after);
    };
    
    // after 'after' time, the pin will be in state 'startingState',
    // then after 'period' time, the pin will be left in state '!startingState'
    int16_t pulse(uint8_t pin, uint8_t startingState, uint32_t period, int32_t after=0){
        return oscillate(pin, startingState, period, 1, after);
    };
    
    // add the event argument, 
    // return event id; if faiiled, return INVALID_EVENT_ID
    int16_t addEvent(EventBase* evt);
    
    // return true if remove success, otherwise return false
    boolean removeEvent(EventBase* evt) { return removeEventByIndex(findEventIndex(evt)); };
    boolean removeEvent(int16_t id) { return removeEvent((EventBase*) id); };
    
    // remove all events,
    // return the count of the events be removed
    int16_t removeAllEvents(void);
    
    // test whether this timer object has the event or not
    boolean hasEvent(EventBase* evt) { return isIndexValid(findEventIndex(evt)); };
    boolean hasEvent(int16_t id) { return hasEvent((EventBase*) id); };
    
private:
    // anything related to event array or index, should not be public
    
    EventBase* events[EVENTS_MAX]; // event array
    
    // coding convention: name 'i' should always denote the index to event array.
    boolean isIndexValid(int8_t i) { return (0 <= i && i < EVENTS_MAX); };

    // pass in event argument, return its index to event array.
    // if pass in NULL, return the index to the free(available) space in the event array.
    int8_t findEventIndex(EventBase* evt);
    int8_t findEventIndex(int16_t id) { return findEventIndex((EventBase*) id); };
    int8_t findFreeEventIndex(void) { return findEventIndex((EventBase*) NULL); };

    boolean removeEventByIndex(int8_t i);
    
    boolean update(uint8_t i);
};

#endif

