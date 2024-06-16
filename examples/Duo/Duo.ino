#include <PServo.h>
#include <Servo.h>

#define __SERVO_PIN_A 7
#define __SERVO_PIN_B 6

unsigned long timer = 0;

ps::PServo myservo_machine_a(&timer);
Servo myservo_a;

ps::PServo myservo_machine_b(&timer);
Servo myservo_b;

void setup(void) {
  myservo_a.attach(__SERVO_PIN_A);
  myservo_b.attach(__SERVO_PIN_B);

  Serial.begin(9600);
}

void loop(void) {
  timer = millis();

  myservo_a.write(myservo_machine_a.get_props().pos);
  myservo_b.write(myservo_machine_b.get_props().pos);

  myservo_machine_a.begin()
      ->move(90, 5)
      ->move(180, 20)
      ->move(0, 5)
      ->move(180, 5)
      ->move(0, 5);

  myservo_machine_b.begin()
      ->move(90, 20)
      ->move(180, 10)
      ->move(0, 30)
      ->move(180, 40)
      ->move(90, 15);
}
