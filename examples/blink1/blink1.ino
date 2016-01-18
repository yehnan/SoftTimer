// One LED blinks. 
// The functionality is the same as the Example - 01.Basics - Blink

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
    
    // arguments: pin, starting state, period, repeatCount, after
    // repeatCount: -1(default) means forever.
    // after: 0(default) means to set the pin right now.
    // timer.ledBlink(LED_PIN, HIGH, PERIOD, -1, 0);
    
    // repeatCount: 10, after 10 cycles(setting he pin to HIGH and LOW), will remove this event
    // after: 5000, the first trigger time is after 5 seconds.
    // timer.ledBlink(LED_PIN, HIGH, PERIOD, 10, 5000);
}

void loop(void)
{
    timer.update();
}
