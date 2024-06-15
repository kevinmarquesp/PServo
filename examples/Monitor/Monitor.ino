#include <PServo.h>

#define __BAUD_RATE 115200
#define __CLOCK_SPEED 750
#define __BUTTON_PIN 9

unsigned long timer = 0;
char logg[64];

inline void display_header(void);
inline void state_logger(char *const buffer, const ps::PServo *const pservo);
inline void main_loop(void);

ps::PServo *const my_servo = new ps::PServo(&timer);

void setup(void) {
  pinMode(__BUTTON_PIN, OUTPUT);

  Serial.begin(__BAUD_RATE);
}

void loop(void) {
  bool const is_stoped = digitalRead(__BUTTON_PIN);

  if (not is_stoped)
    return;

  state_logger(logg, my_servo);
  Serial.println(logg);

  main_loop();
  delay(__CLOCK_SPEED);

  ++timer;
}

inline void main_loop(void) {}

inline void state_logger(char *const buff, const ps::PServo *const pservo) {
  using namespace ps;

  char const *s = state_text(pservo->get_state());
  Props const p = pservo->get_props();

  sprintf(buff, "[%.4d|%.4d] %3dº with %.4d, %2d/%d in %s",
          (unsigned short)timer, (unsigned short)p.pc, p.pos, p.delay,
          p.curr_action, p.actions_count, s);
}
