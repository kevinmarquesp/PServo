#pragma once

namespace ps {
enum class State : unsigned char {
  STANDBY,
  INITIALIZED,
  MOVING,
  WAITING,
  DONE,
  PAUSED
};

namespace Default {
unsigned char const MIN = 0;
unsigned char const MAX = 180;
unsigned char const DELAY = 1;
}; // namespace Default

typedef struct Props {
  unsigned long pc;

  unsigned char min;
  unsigned char max;
  bool is_resetable;

  unsigned char curr_action;
  unsigned char actions_count;
  unsigned char pos;
  unsigned short delay;
} Props;

class PServo {
public:
  PServo() {}
  PServo(bool const is_resetable) : _is_resetable(is_resetable) {}
  PServo(unsigned char const min, unsigned char const max)
      : _min(min), _max(max) {}
  PServo(unsigned char const min, unsigned char const max,
         bool const is_resetable)
      : _min(min), _max(max), _is_resetable(is_resetable) {}

  void refresh(unsigned long *const timer);
  unsigned char get_pos(void);

  PServo *begin(void);
  PServo *wait(bool proceed);
  PServo *move(unsigned char const next_pos);
  PServo *move(unsigned char const next_pos, unsigned short const delay);
  PServo *halt(void);

  Props get_props(void);
  State get_state(void);

private:
  State _state = State::STANDBY;

  unsigned long _pc = 0;
  unsigned long *_timer = nullptr;

  unsigned char _min = Default::MIN;
  unsigned char _max = Default::MAX;
  bool _is_resetable = false;

  unsigned char _curr_action = 0;
  unsigned char _actions_count = 0;
  unsigned char _pos = 0;
  unsigned short _delay = Default::DELAY;
};

char const *state_text(State s);
}; // namespace ps
