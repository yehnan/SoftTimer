// Use switch to control LED blinking

#include <SoftTimer.h>

#define LED_PIN 13
#define SWITCH_PIN 2

#define PERIOD 1000  // 1 second

int idLedEvent = INVALID_EVENT_ID;

SoftTimer timer;  // create the timer object

void setup(void)
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT); // assume HIGH means pressed
    
    idLedEvent = timer.ledBlink(LED_PIN, HIGH, PERIOD);    
}

void loop(void)
{
    timer.update();
    
    if(digitalRead(SWITCH_PIN)){ // assume HIGH means pressed
        if(idLedEvent == INVALID_EVENT_ID){
            idLedEvent = timer.ledBlink(LED_PIN, HIGH, PERIOD);   
        }
        else{
            timer.removeEvent(idLedEvent);
            idLedEvent = INVALID_EVENT_ID;
        }
    }
}
