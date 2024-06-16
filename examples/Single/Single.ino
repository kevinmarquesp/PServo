#include <PServo.h>
#include <Servo.h>

#define __SERVO_PIN 7

unsigned long timer = 0;

ps::PServo myservo_machine(&timer);
Servo myservo;

void setup(void) {
  myservo.attach(__SERVO_PIN);

  Serial.begin(9600);
}

void loop(void) {
  timer = millis();

  myservo.write(myservo_machine.get_props().pos);

  myservo_machine.begin()
      ->move(90, 15)
      ->move(180, 40)
      ->move(0, 30)
      ->move(180, 10)
      ->move(0, 10);
}
