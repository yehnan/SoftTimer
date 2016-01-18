// Two LEDs blink at different rates 

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
    
    // arguments: pin, starting value, peroid, repeat count, after
    // repeat count: -1 means forever
    // after: 0, means the first trigger time is after 0 millisecond, i.e. now.
    // timer.oscillate(LED1_PIN, HIGH, PERIOD1, -1, 0);
    // timer.oscillate(LED2_PIN, HIGH, PERIOD2, -1, 0);
}

void loop(void)
{
    timer.update();
}
