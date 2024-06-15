#include "PServo.h"

ps::PServo *ps::PServo::begin(void) {
  using namespace ps;

  switch (_state) {
  case State::STANDBY: // Initialize the machine action counter before run.
    _state = State::INITIALIZED;
    break;

  case State::INITIALIZED: // Here, it will be ready to start the movements.
    if (_actions_count < 1) {
      _state = State::ERROR_NOACTION;
      break;
    }

    _reset_active_action_then_move();
    break;

  case State::DONE: // Reset or stop the machine if done, or goto next movement.
    if (_active_action > _actions_count) {
      if (_is_resetable)
        _reset_active_action_then_move();
      else
        _state = State::HALT;
    }

    break;

  case State::PAUSED: // To keep pause, don't do anything, just update _pc.
    if (_timer == nullptr) {
      _state = State::ERROR_TIMERPTR;
      break;
    }

    _pc = *_timer;
    break;

  case State::HALT:
    break;
  case State::ERROR_NOACTION:
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  return this;
}

inline void ps::PServo::_reset_active_action_then_move(void) {
  if (_actions_count < 1) {
    _state = State::ERROR_NOACTION;
    return;
  }

  _state = State::IN_ACTION;
  _active_action = 0;
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
      _state = State::DONE;
      break;
    }

    _delay = delay;

    if (*_timer - _pc > delay) {
      _pc = *_timer;
      _pos = _pos < next_pos ? _pos + 1 : _pos - 1;
    }

    break;

  case State::PAUSED:
    break;
  case State::HALT:
    break;
  case State::ERROR_NOACTION:
    break;

  default:
    _state = State::ERROR_UNEXPECTED;
  }

  return this;
}

ps::PServo *ps::PServo::move(unsigned char const next_pos) {
  using namespace ps;

  return this->move(next_pos, Default::DELAY);
}

ps::Props ps::PServo::get_props(void) {
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

ps::State ps::PServo::get_state(void) { return _state; }

char const *ps::state_text(ps::State s) {
  using namespace ps;

  // Vim macro that may help some developers...
  // :nnoremap UU $dlIs == State::<esc>lyiwA ? "<esc>pA" :<esc>j0

  return s == State::STANDBY            ? "STANDBY"
         : s == State::INITIALIZED      ? "INITIALIZED"
         : s == State::HALT             ? "HALT"
         : s == State::IN_ACTION        ? "IN_ACTION"
         : s == State::DONE             ? "DONE"
         : s == State::PAUSED           ? "PAUSED"
         : s == State::ERROR_UNEXPECTED ? "ERROR_UNEXPECTED"
         : s == State::ERROR_NOACTION   ? "ERROR_NOMOVE"
         : s == State::ERROR_TIMERPTR   ? "ERROR_TIMERPTR"
                                        : "";
}
