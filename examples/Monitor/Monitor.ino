#include <PServo.h>

#define __BAUD_RATE 115200
#define __DELAY 1500

ps::PServo *servo_handler;

void setup(void) {
  servo_handler = new ps::PServo();

  Serial.begin(__BAUD_RATE);
}

void loop(void) {
  ps::State const curr_state = servo_handler->get_state();

  Serial.println((unsigned char)curr_state);
  delay(__DELAY);
}
