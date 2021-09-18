#include "EventBase.h"

EventBase::EventBase(void)
{
    // use object's address as event id
    id = (intptr_t) this;
    
    repeatCount = 0;
    period = 0;
    nextTriggerTime = 0;
    callback = (CB_TYPE) NULL;
    
    deleteWhenRemove = false;
}

