// One LED blinks, the time of HIGH and LOW are different 
// use two callback functions to achieve the above requirement.

#include "SoftTimer.h"

// the pin connecting to a LED, don't forget the current-limiting resistor
#define LED_PIN 13

#define PERIOD_HIGH 2500  // 2.5 second
#define PERIOD_LOW 500  // 0.5 second

SoftTimer timer;  // create the timer object

boolean led_off(EventBase* evt)
{
    digitalWrite(LED_PIN, LOW);
    timer.once(led_on, PERIOD_LOW);
    
    return false;
}

boolean led_on(EventBase* evt)
{
    digitalWrite(LED_PIN, HIGH); // light on
    
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
    
    // after PERIOD_LOW milliseconds, will call callback function led_on once
    timer.once(led_on, PERIOD_LOW);
}

void loop(void)
{
    timer.update();
}
