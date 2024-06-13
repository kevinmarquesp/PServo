#include <PServo.h>

#define __BAUD_RATE 115200
#define __DELAY 1500

PServo::PServo *servo_handler;

void setup(void) {
  servo_handler = new PServo::PServo();

  Serial.begin(__BAUD_RATE);
}

void loop(void) {
  Serial.println("Hello world!");

  delay(__DELAY);
}
