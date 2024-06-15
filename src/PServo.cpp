#include "PServo.h"

ps::PServo *ps::PServo::begin(unsigned long const timer) {
  using namespace ps;

  _timer = timer;

  switch (_state) {
  case State::STANDBY: // Initialize the machine action counter before run.
    _state = State::INITIALIZED;
    break;

  case State::HALTED:
    _state = State::MOVING;
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  return this;
}
ps::PServo *ps::PServo::move(unsigned char const next_pos,
                             unsigned short const delay) {
  using namespace ps;

  switch (_state) {
  case State::INITIALIZED: // Count actions ammount before the first halt.
    ++_actions_count;
    break;

  case State::MOVING: // Start the async timer for the current action.
                      // TODO!
    break;

  case State::DONE: // Pass to the next action number, then move again.
                    // TODO!
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  return this;
}

ps::PServo *ps::PServo::move(unsigned char const next_pos) {
  return this->move(next_pos, _delay);
}

ps::PServo *ps::PServo::halt(void) {
  using namespace ps;

  switch (_state) {
  case State::INITIALIZED: // Stop the initialization.
    _state = State::HALTED;
    break;

  case State::DONE: // Reset or stop the machine if it's in the last action.
                    // TODO!
    break;

  case State::MOVING:
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  return this;
}

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
