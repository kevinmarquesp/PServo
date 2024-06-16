#pragma once

namespace ps {
enum class State : unsigned char {
  STANDBY,
  INITIALIZED,
  HALT,
  IN_ACTION,
  PAUSED,
  ERROR_UNEXPECTED,
  ERROR_NOACTION,
  ERROR_TIMERPTR,
};

namespace Default {
unsigned char const MIN = 0;
unsigned char const MAX = 180;
unsigned char const DELAY = 1;
}; // namespace Default

typedef struct Props {
  State state;

  unsigned long pc;
  unsigned long *const timer;

  unsigned char min;
  unsigned char max;
  bool is_resetable;

  unsigned char curr_action;
  unsigned char active_action;
  unsigned char actions_count;
  unsigned char pos;
  unsigned short delay;
} Props;

class PServo {
public:
  PServo(unsigned long *const timer) : _timer(timer) {}

  PServo(unsigned long *const timer, bool const is_resetable)
      : _timer(timer), _is_resetable(is_resetable) {}

  PServo(unsigned long *const timer, unsigned char const min,
         unsigned char const max)
      : _timer(timer), _min(min), _max(max) {}

  PServo(unsigned long *const timer, unsigned char const min,
         unsigned char const max, bool const is_resetable)
      : _timer(timer), _min(min), _max(max), _is_resetable(is_resetable) {}

  PServo *begin(void);
  PServo *move(unsigned char const next_pos, unsigned short const delay);
  PServo *move(unsigned char const next_pos);

  Props get_props(void);
  State get_state(void);

private:
  State _state = State::STANDBY;

  unsigned long _pc = 0;
  unsigned long *const _timer = nullptr;

  unsigned char _min = Default::MIN;
  unsigned char _max = Default::MAX;
  bool _is_resetable = false;

  unsigned char _curr_action = 0;
  unsigned char _active_action = 0;
  unsigned char _actions_count = 0;
  unsigned char _pos = 0;
  unsigned short _delay = 0;

  inline void _reset_active_action_to_start_again(void);
  inline void _reset_or_update_and_start_next_action(void);
};

char const *state_text(State s);
}; // namespace ps
