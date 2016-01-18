#include "SoftTimer.h"

#define PIN 13

SoftTimer timer;

void setup()
{
    pinMode(PIN, OUTPUT);
    
    timer.pulse(PIN, HIGH, 10*60*1000); // HIGH for 10 minutes from now
    
    // timer.pulse(PIN, HIGH, 5*1000); // HIGH for 5 seconds from now
    
    // timer.pulse(PIN, HIGH, 5*1000, 2*1000); // HIGH for 5 seconds after 2 seconds
}

void loop()
{
    timer.update();
}

