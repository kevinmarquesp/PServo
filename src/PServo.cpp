#include "PServo.h"

ps::Props ps::PServo::get_props(void) {
  using namespace ps;

  return Props{
      .pc = _pc,
      .min = _min,
      .max = _max,
      .is_resetable = _is_resetable,
      .curr_action = _curr_action,
      .actions_count = _actions_count,
      .pos = _pos,
      .delay = _delay,
  };
}

ps::State ps::PServo::get_state(void) { return _state; }

char const *ps::state_text(ps::State s) {
  using namespace ps;

  return s == State::STANDBY       ? "STANDBY"
         : s == State::INITIALIZED ? "INITIALIZED"
         : s == State::MOVING      ? "MOVING"
         : s == State::WAITING     ? "WAITING"
         : s == State::DONE        ? "DONE"
         : s == State::PAUSED      ? "PAUSED"
                                   : "UNKNOWN";
}
