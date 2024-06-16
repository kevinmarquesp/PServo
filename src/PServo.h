#pragma once

/*!
 * Precise servo. Holds the core classes, functions and constants related to the
 * state machine that will control the position (it wont write anything
 * anywere because it doesn't depends on the `Arduino.h` library).
 */
namespace ps {

/*!
 * This class members has all the possible states for the `ps::PServo` state
 * machine. Use it along side with the `ps::PServo::get_state()` method to build
 * a more complex logic to your sketch.
 *
 * Since it's each state is, in fact, a `unsigned char`. Maybe you'll need to
 * cast it to a integer type if you want to print the state number on the serial
 * port.
 *
 * For an example:
 * ```cpp
 * if (myservo_machine.get_state() == State::HALT) {
 *   Serial.println("All movements was completed!");
 *
 *   Serial.print("State ID: ");
 *   Serial.println((int)state);
 * }
 * ```
 */
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

/*!
 * Normally the user doesn't need this values at all, they're here to construct
 * the `ps::PServo` object with these default values...
 */
namespace Default {
unsigned char constexpr MIN = 0;
unsigned char constexpr MAX = 180;
unsigned char constexpr DELAY = 1;
}; // namespace Default

/*!
 * List of all the private properties of `ps::PServo`. It's primary useful for
 * testing and monitoring strategies, but be aware that you cannot hack those
 * values inside the class, those are read only througth this struct.
 */
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

/*!
 * Main class of the library, represents the **state machine** of an asyncronous
 * servo motor object. Which means that this can hold the position (in deg)
 * inside it, so you can get use that information to wirte in a real servo motor
 * component.
 *
 * Since it uses an asyncronous logic, you need to pass the reference of an
 * counter, and this counter should be be updated every loop iteration (see the
 * example).
 *
 * The logic to make it work is quite simple, start with the `PServo::being()`
 * call, so it knows where set of movements needs to stop and start other for
 * each loop, then put your sequence of movements that you want to perform one
 * after another. This machine is smart enough to figure out which movement
 * should be executed and which should be just ignored.
 *
 * For an example:
 * ```cpp
 * unsigned long timer = 0;
 *
 * ps::PServo myservo_machine(&timer);
 * Servo myservo;
 *
 * void setup() {
 *   Servo.attach(8);
 * }
 *
 * void loop() {
 *   timer = millis();
 *
 *   myservo.write(myservo_machine.get_props().pos);
 *
 *   myservo_machine.begin()
 *     ->move(90, 10)
 *     ->move(180, 25)
 *     ->move(0, 5);
 * }
 * ```
 */
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

  Props const get_props(void) const;
  State const get_state(void) const;

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

/*!
 * Return the specified state name in a string. Mainly used for debug and
 * monitoring propurses.
 *
 * For an example:
 * ```cpp
 * ps::State const current_state = myservo_machine.get_state();
 *
 * Serial.print("Current state: ");
 * Serial.println(ps::state_text(current_state));
 * ```
 */
char const *state_text(State s);
}; // namespace ps
