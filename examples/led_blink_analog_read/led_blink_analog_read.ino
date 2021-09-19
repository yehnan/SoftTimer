#include "SoftTimer.h"

#define LED_PIN 13
#define READ_PIN A0

SoftTimer timer;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    
    timer.ledBlink(LED_PIN, LOW, 300);
    timer.every(1000, callbackReadAnalog);
}

void loop()
{
    timer.update();
}

bool callbackReadAnalog(EventBase* evt)
{
    Serial.println(analogRead(READ_PIN));
    
    return false;
}
