# Arduino SoftTimer Library
A soft timer library for Arduino  
https://github.com/yehnan/SoftTimer


## History
There are hardware timer libraries for Arduino, for example, TimerOne, TimerThree, and FlexTimer2. But we don't always want to mess with hardware timer, only a soft timer library is needed. Like Timer by Dr. Simon Monk(http://www.doctormonk.com/2012/01/arduino-timer-library.html) which has been improved by Jack Christensen and other contributors. Please see https://github.com/JChristensen/Timer/tree/v1.3 and https://github.com/JChristensen/Timer/tree/v2.1 for more information.

The soft timer library does not interfere with the built-in hardware timers, it just uses `millis()` in a basic type of scheduler to decide when something needs doing.

I have been used Timer library for a long time and for various kinds of tasks. Although desired to improve it, found that it's very hard to add the wanted features without breaking backwards compatibility. So I decided to rewrite from scratch. Hope you will find it useful.


##Examples

The first Arduino course is usually File - Examples - 01.Basics - Blink. It's good and ok for showing beginners how to make an LED blink(flash). But soon you will start feel that `delay()` is a curse. It stops the microcontroller and prevents other things from executing.

```c++
void setup() {
    pinMode(13, OUTPUT);
}

void loop() {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}
```

When you call `delay(1000)`, within that one second, nothing else can happen. You can't update a display, read serial port, or check for key presses for example.

Let's write the same functionality using **SoftTimer** library:

```c++
#include "SoftTimer.h"

// the pin connecting to a LED, don't forget the current-limiting resistor
#define LED_PIN 13

#define PERIOD 1000  // 1 second

SoftTimer timer;  // create the timer object

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    
    // arguments: pin, starting state, period
    timer.ledBlink(LED_PIN, HIGH, PERIOD);
}

void loop(void)
{
    timer.update();
}

```

The `ledBlink()` method takes arguments of a pin to change, the starting state, and the period to change it.

The call to `timer.update()` takes a matter of microseconds to run, and when the appropriate period of time has passed, it will change the state of the pin.

Following is another example that uses two timer events to flash two LEDs at different rates.

```c++
#include "SoftTimer.h"

// the pins for two LEDS, don't forget the current-limiting resistors
#define LED1_PIN 13
#define LED2_PIN 12

#define PERIOD1 500  // 0.5 second
#define PERIOD2 3000  // 3 seconds

SoftTimer timer;  // create the timer object

void setup(void)
{
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    
    // arguments: pin, starting value, peroid
    timer.ledBlink(LED1_PIN, HIGH, PERIOD1);
    timer.ledBlink(LED2_PIN, HIGH, PERIOD2);
}

void loop(void)
{
    timer.update();
}
```

The following example uses one timer object to set two events independent. One for LED blinking, another one for reading analog pin.

```c++
#include "SoftTimer.h"

#define LED_PIN 13
#define READ_PIN A0

SoftTimer timer;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    
    timer.ledBlink(LED_PIN, LOW, 300);     // LED blink
    timer.every(1000, callbackReadAnalog); // call callback function periodically
}

void loop()
{
    timer.update();
}

boolean callbackReadAnalog(EventBase* evt)
{
    Serial.println(analogRead(READ_PIN));
    
    return false; // remember to return false, means you want to event to continue
}
```

The following example use two callback functions. One will light on LED, another one will light off LED. Each for different time duration.

```c++
#include "SoftTimer.h"

// the pin connecting to a LED, don't forget the current-limiting resistor
#define LED_PIN 13

#define PERIOD_HIGH 2500  // 2.5 second
#define PERIOD_LOW 500  // 0.5 second

SoftTimer timer;  // create the timer object

boolean led_off(EventBase* evt)
{
    digitalWrite(LED_PIN, LOW);
    
    // in callback function, you can create another event
    // after PERIOD_LOW milliseconds, will call callback function led_on once
    timer.once(led_on, PERIOD_LOW);
    
    return false;
}

boolean led_on(EventBase* evt)
{
    digitalWrite(LED_PIN, HIGH); // light on
    
    // in callback function, you can create another event
    // after PERIOD_HIGH milliseconds, will call callback function led_off once
    timer.once(led_off, PERIOD_HIGH);
    
    // if return true, means to remove this event.
    // if return false, let the timer object handle it based on original settings
    return false;
}

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    timer.once(led_on, PERIOD_LOW);
}

void loop(void)
{
    timer.update();
}
```

The next example is harder. Because we want to control servo, need to define new event class to contain more data. The functionality is the same as the Example - Servo - Sweep


```c++
#include <SoftTimer.h>
#include <Servo.h>

// the pin connecting to a servo
#define SERVO_PIN 9

#define DELAY 15
#define INCVALUE 1

SoftTimer timer;  // create the timer object

class EventServo : public EventBase
{
public:
    Servo myservo;
    int16_t position;
    int16_t incValue;
};

EventServo event_servo;

static boolean callback_servo_position(EventBase* evt)
{
    EventServo* e = (EventServo*) evt;
    
    e->position += e->incValue;
    if(e->position < 0){
        e->position = 0;
        e->incValue = -(e->incValue);
    }
    else if(180 < e->position){
        e->position = 180;
        e->incValue = -(e->incValue);
    }
    e->myservo.write(e->position);
    
    return false;
}

void setup() 
{ 
    event_servo.myservo.attach(SERVO_PIN);
    
    event_servo.period = DELAY;
    event_servo.repeatCount = -1; // forever
    event_servo.period = DELAY;
    event_servo.nextTriggerTime = 0; // if 0, means now
    event_servo.callback = &callback_servo_position;

    event_servo.position = 0; // starting position
    event_servo.incValue = INCVALUE;
    
    timer.addEvent(&event_servo);
} 
 
void loop() 
{ 
    timer.update();
} 

```

There are other examples. Please see the code and comments.


##Installation

As with all other Arduino libraries, unzip the file into the *libraries* folder in your Arduino sketchbook directory, which will be something like *My Documents\Arduino\sketchbook* on Windows, *Documents/Arduino/sketchbook* on Mac etc.  If this is the first library you have installed, you will need to create a directory there called *libraries*.

The SoftTimer library is compatible with both Arduino 1.0+ and earlier versions.


##Reference

###EVENTS_MAX
#####Description
The max count of events a timer object can have.
Default is 10. Users can modify it if you need more or less. However, smaller value won't save you much of memory. The library dynamically manage the memory for the event. 

###INVALID_EVENT_ID
#####Description
Users can/should use this macro constant to denote invalid event id. Valid event id should be a positive value.

###update();
#####Description
Must be called from `loop()`. This will process all the events associated with the timer. Check time and determine whether the event should be triggered or not.
#####Syntax
`timer.update();`
#####Parameters
None.
#####Returns
return the count of events be triggered in this update() call.

###int16_t oscillate(uint8_t pin, uint8_t startingState, uint32_t period, int16_t repeatCount=-1, int32_t after=0);
#####Description
Change pin state periodically
#####Syntax
`timer.oscillate(pin, startingState, period);`  
`timer.oscillate(pin, startingState, period, repeatCount, after);`  
#####Parameters
***pin:*** The pin whose state you want to change 
***startingState:*** The starting state   
***period:*** The time duration between two event triggering. In milliseconds.
***repeatCount:*** How many times to trigger the event. Optional, default value is -1, means forever. Note: for oscillate, this argument means one full cycle(for example, from HIGH to LOW then to HIGH), not partial transition(for example, from HIGH to LOW).
***after:*** When is the first time to trigger the event. Optional, default value is 0, means now. If positive, the first time to trigger is now + *after*. If negative, the first time to trigger is now + *period*.
#####Returns
If the timer event is successfully created, return its id ; if failed, return INVALID_EVENT_ID

###int16_t ledBlink(uint8_t pin, uint8_t startingState, uint32_t period, int16_t repeatCount=-1, int32_t after=0);
#####Description
identical to oscillate(pin, startingState, period, repeatCount, after);

###int16_t pulse(uint8_t pin, uint8_t startingState, uint32_t period, int32_t after=0);
#####Description
identical to oscillate(pin, startingState, period, 1, after);

###int16_t schedule(uint32_t period, CB_TYPE callback, int16_t repeatCount=-1, int32_t after=0);
#####Description
Do something(call/execute callback function) periodically
#####Syntax
`timer.schedule(period, callback);`  
`timer.schedule(period, callback, repeatCount, after);`
#####Parameters
***period:*** The time duration between two event triggering. In milliseconds. 
***callback:*** The name of the callback function which will be called when triggering the event. You should pass in a function pointer of prototype boolean (*)(EventBase*).

Note: the callback gets the event as argument. 
If callback returns true, the event will be removed; if returns false, just continue as usual.
The canonical implementation is:
```c++
boolean callback_for_some_event(EventBase* evt)
{
    return false; // !don't forget! or the C language's behavior is undefined
}
```
    
***after:*** When is the first time to trigger the event. Optional, default value is 0, means now. If positive, the first time to trigger is now + *after*. If negative, the first time to trigger is now + *period*.
#####Returns
If the timer event is successfully created, return its id ; if failed, return INVALID_EVENT_ID

###int16_t every(uint32_t period, CB_TYPE callback, int32_t after=0);
#####Description
Do something(call/execute callback function) periodically.
Identical to schedule(period, callback, -1, after);


###int16_t once(CB_TYPE callback, int32_t after);
#####Description
Do something(call/execute callback function) only once.
Identical to schedule(0, callback, 1, after);

###int16_t addEvent(EventBase* evt);
#####Description
Create the event by yourself, then use this method to add the event to the timer object
#####Syntax
`timer.addEvent(evt);`  
#####Parameters
***evt:*** pointer to an event object of class EventBase(or subclass)

```c++
class EventBase
{
public:
    EventBase(void);
    
    int16_t id;
    
    int repeatCount;
    uint32_t period;
    uint32_t nextTriggerTime;
    
    CB_TYPE callback;

    boolean deleteWhenRemove;
};
```

***id***: The unique id of the event. Created automatically by the class. You don't need to worry about it.
***repeatCount***: Negative value(like -1) means forever; 0 means to be removed; positive value means the times to call the callback function.
***period***: The time duration between two event triggering. In milliseconds. Advanced users can utilize it, i.e., set to different value in the callback function to affect the next event trigger time.
***nextTriggerTime***: The timer object uses this to decide whether to trigger the event or not. It is usually determined by *period*, so you don't need to worry about it.
***callback***: The name of the callback function which will be called when triggering the event. A function pointer of prototype boolean (*)(EventBase*).
***deleteWhenRemove***: When the timer object is going to remove the event, does it have to delete the event? Usually set to false(default), means the timer object won't touch the event.
#####Returns
If the timer event is successfully added, return its id *(int16_t)*; if failed, return INVALID_EVENT_ID

###boolean removeEvent(EventBase* evt);
#####Description
remove the event denoted by the argument(pointer to event object)
#####Syntax
`timer.removeEvent(evt);`  
#####Parameters
***evt:*** pointer to the event object of class EventBase(or subhclass) 
#####Returns
return true if succeed, otherwise return false

###boolean removeEvent(int16_t id);
#####Description
remove the event denoted by the argument(event id)
#####Syntax
`timer.removeEvent(id);`  
#####Parameters
***id:*** event id
#####Returns
return true if succeed, otherwise return false

###int16_t removeAllEvents(void);
#####Description
remove all events
#####Syntax
`timer.removeAllEvents();`  
#####Returns
return the count of the events be removed

###boolean hasEvent(EventBase* evt);
#####Description
test whether this timer object has the event or not
#####Syntax
`timer.hasEvent(evt);`  
#####Parameters
***evt:*** pointer to the event object of class EventBase(or subhclass) 
#####Returns
return true if succeed, otherwise return false

###boolean hasEvent(int16_t id);
#####Description
test whether this timer object has the event or not
#####Syntax
`timer.hasEvent(id);`  
#####Parameters
***id:*** event id
#####Returns
return true if succeed, otherwise return false

For more details please see the comments in source files.


##Revision History

This SoftTimer library is currently in beta test version. Please test and use it. Any suggestion and feeback is welcome.


