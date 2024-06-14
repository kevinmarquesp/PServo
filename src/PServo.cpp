#include "PServo.h"

using namespace ps;

Props PServo::get_props(void) {
  return Props{
      .min = _min,
      .max = _max,
      .is_resetable = _is_resetable,
      .curr_action = _curr_action,
      .actions_count = _actions_count,
      .pos = _pos,
      .delay = _delay,
  };
}

State PServo::get_state(void) { return _state; }
