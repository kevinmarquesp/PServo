#pragma once

namespace PServo {
enum class State : unsigned char {
  STAND_BY,
  INITIALIZED,
  MOVING,
  WAITING,
  DONE,
  PAUSED,
  ERROR_UNINITIALIZED
};

namespace Default {
unsigned char const MIN = 0;
unsigned char const MAX = 180;
unsigned char const DELAY = 1;
}; // namespace Default

typedef struct Props {
  State state;
  bool is_status_ok;

  unsigned char min;
  unsigned char max;
  bool is_resetable;

  unsigned char curr_action;
  unsigned char actions_count;
  unsigned char pos;
  unsigned short delay;
} props;

class PServo {
public:
  PServo(unsigned char const min, unsigned char const, bool max)
      : _min(min), _max(max) {}
  PServo() {}

  void refresh(unsigned long const *timer);
  unsigned char get_pos(void);

  PServo *begin(void);
  PServo *wait(bool proceed);
  PServo *move(unsigned char const next_pos);
  PServo *move(unsigned char const next_pos, unsigned short const delay);
  PServo *halt(void);

  props get_props(void);
  State get_state(void);
  bool is_status_ok(void);

private:
  State _state = State::STAND_BY;
  bool _is_status_ok = true;

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
}; // namespace PServo
