#ifndef __Event_Base_h__
#define __Event_Base_h__

// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

class EventBase;

// callback function type
// if callback returns true, will remove the event;
// if returns false, continue...
typedef bool (*CB_TYPE)(EventBase*);

class EventBase
{
public:
    EventBase(void);
    
    intptr_t id;
    
    int repeatCount; // negative(like -1): forever, 0: stopped, 
    uint32_t period;
    uint32_t nextTriggerTime;
    
    CB_TYPE callback;

    bool deleteWhenRemove; // if true, let class Timer handle the memory management 
};

class EventPinState : public EventBase
{
public:
    uint8_t pin;
    uint8_t state;
};

#endif

