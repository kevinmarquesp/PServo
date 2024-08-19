/*!
 * MIT License
 *
 * Copyright (c) 2024 Kevin Marques
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * Classes and functions declarations
 * ----------------------------------
 */

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
unsigned char constexpr MIN = 0;
unsigned char constexpr MAX = 180;
unsigned char constexpr DELAY = 1;
}; // namespace Default

typedef struct Props {
  State state;
  unsigned long pc;
  unsigned long *const timer;
  unsigned char const min;
  unsigned char const max;
  bool const is_resetable;
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
  PServo *move(unsigned char const next_pos);
  PServo *move(unsigned char const next_pos, unsigned short const delay);
  Props const props(void) const;
  State const get_state(void) const;
  bool is_state(State s) const;
  unsigned char pos(void) const;
  void reset(void);

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
  unsigned short _delay = Default::DELAY;

  inline void _reset_active_action_to_start_again(void);
  inline void _reset_or_update_and_start_next_action(void);
};

char const *state_text(State s);
}; // namespace ps

/*!
 * Method definitions
 * ------------------
 */

ps::PServo *ps::PServo::begin(void) {
  using namespace ps;

  _curr_action = 0;

  switch (_state) {
  case State::STANDBY:
    _state = State::INITIALIZED;
    break;

  case State::INITIALIZED:
    if (_actions_count < 1) {
      _state = State::ERROR_NOACTION;
      break;
    }

    _reset_active_action_to_start_again();
    break;

  case State::PAUSED:
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
  case State::INITIALIZED:
    ++_actions_count;
    break;

  case State::IN_ACTION:
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
  if (_actions_count < 1) {
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
