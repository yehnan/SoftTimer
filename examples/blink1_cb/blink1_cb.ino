// Use callback to blink LED, and output LED status to serial port

#include <SoftTimer.h>

#define LED_PIN 13

#define PERIOD 1000  // 1 second

int led_state = HIGH;

SoftTimer timer;  // create the timer object

boolean callback(EventBase* evt)
{
    Serial.print("LED state = ");
    Serial.println(led_state);
    
    digitalWrite(LED_PIN, led_state);
    led_state = !led_state;
    
    return false;
}

void setup(void)
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    
    // repeatedly call 'callback' every 'PERIOD' time
    timer.schedule(PERIOD, &callback);
    
    // after '5000' milliseconds, first time to call 'callback',
    // after '5' times, remove this event.
    // timer.schedule(PERIOD, &callback, 5, 5000);
    
    // same as timer.schedule(PERIOD, &callback);
    // timer.every(PERIOD, &callback);
    
    // after '5000' milliseconds, first time to call 'callback',
    // then repeatedly call 'callback' every 'PERIOD' time.
    // timer.every(PERIOD, &callback, 5000);
}

void loop(void)
{
    timer.update();
}
