#include "PServo.h"

ps::PServo *ps::PServo::begin(void) {
  using namespace ps;

  _curr_action = 0;

  switch (_state) {
  case State::STANDBY: // Initialize the machine action counter before run.
    _state = State::INITIALIZED;
    break;

  case State::INITIALIZED: // Here, it will be ready to start the movements.
    if (_actions_count < 1) {
      _state = State::ERROR_NOACTION;
      break;
    }

    _reset_active_action_to_start_again();
    break;

  case State::PAUSED: // To keep pause, don't do anything, just update _pc.
    if (_timer == nullptr) {
      _state = State::ERROR_TIMERPTR;
      break;
    }

    _pc = *_timer;
    break;

  case State::IN_ACTION:
  case State::HALT:
  case State::ERROR_NOACTION:
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

  case State::IN_ACTION: // Start the async timer for the current action.
    if (_timer == nullptr) {
      _state = State::ERROR_TIMERPTR;
      break;
    }

    if (_active_action != _curr_action)
      break;

    if (_pos == next_pos) {
      _reset_or_update_and_start_next_action();
      break;
    }

    _delay = delay < Default::DELAY ? Default::DELAY : delay;

    if (*_timer - _pc >= delay) {
      _pc = *_timer;
      _pos = _pos < next_pos ? _pos + 1 : _pos - 1;
      _pos = _pos < _min ? _min : _pos > _max ? _max : _pos;
    }

    break;

  case State::PAUSED:
  case State::HALT:
  case State::ERROR_NOACTION:
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  ++_curr_action;

  return this;
}

inline void ps::PServo::_reset_or_update_and_start_next_action(void) {
  ++_active_action;

  if (_active_action >= _actions_count) {
    if (_is_resetable)
      _reset_active_action_to_start_again();
    else
      _state = State::HALT;

    return;
  }

  _state = State::IN_ACTION;
}

inline void ps::PServo::_reset_active_action_to_start_again(void) {
  if (_actions_count < 1) { // This condition is useful for the first
                            // PServo::begin() call.
    _state = State::ERROR_NOACTION;
    return;
  }

  _state = State::IN_ACTION;
  _active_action = 0;
}

ps::PServo *ps::PServo::move(unsigned char const next_pos) {
  using namespace ps;

  return this->move(next_pos, Default::DELAY);
}

ps::Props const ps::PServo::props(void) const {
  using namespace ps;

  return Props{
      .state = _state,
      .pc = _pc,
      .timer = _timer,
      .min = _min,
      .max = _max,
      .is_resetable = _is_resetable,
      .curr_action = _curr_action,
      .active_action = _active_action,
      .actions_count = _actions_count,
      .pos = _pos,
      .delay = _delay,
  };
}

void ps::PServo::reset(void) {
  if (_state != State::HALT)
    return;

  _state = State::STANDBY;
  _active_action = 0;
  _actions_count = 0;
}

ps::State const ps::PServo::get_state(void) const { return _state; }

bool ps::PServo::is_state(ps::State s) const { return _state == s; }

unsigned char ps::PServo::pos(void) const { return _pos; }

char const *ps::state_text(ps::State s) {
  using namespace ps;

  // Vim macro that may help some developers...
  // :nnoremap UU $dlIs == State::<esc>lyiwA ? "<esc>pA" :<esc>j0

  return s == State::STANDBY            ? "STANDBY"
         : s == State::INITIALIZED      ? "INITIALIZED"
         : s == State::HALT             ? "HALT"
         : s == State::IN_ACTION        ? "IN_ACTION"
         : s == State::PAUSED           ? "PAUSED"
         : s == State::ERROR_UNEXPECTED ? "ERROR_UNEXPECTED"
         : s == State::ERROR_NOACTION   ? "ERROR_NOMOVE"
         : s == State::ERROR_TIMERPTR   ? "ERROR_TIMERPTR"
                                        : "";
}
