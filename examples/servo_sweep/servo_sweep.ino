// The functionality is the same as the Example - Servo - Sweep

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

