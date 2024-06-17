#pragma once

/*!
 * Precise servo. Holds the core classes, functions and constants related to the
 * state machine that will control the position (it wont write anything
 * anywere because it doesn't depends on the `Arduino.h` library).
 *
 * When used in `.ino` files, it is recommended that the developer consider
 * **not** using this namespace globally, the names inside this namespace may
 * cause some confusion without the namespace's name as a context. Only inside a
 * function definition it's OK to use `using namespace ps;` at the top.
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
 *
 * > **Note**: The *NOOP* means that the state will not perform any action nor
 * > update any value inside the machine. The responsability to handle and
 * > update to a valid state will be at the user's hand.
 *
 * @see ps::PServo
 */
enum class State : unsigned char {
  STANDBY,          //!< Will count each action on this state.
  INITIALIZED,      //!< Measn that the actions was counted, so start it!
  HALT,             //!< No operation, the final action was completed (*NOOP*).
  IN_ACTION,        //!< Will keep updating the servo's position.
  PAUSED,           //!< No operation, but keep updating the `_pc` var (*NOOP*).
  ERROR_UNEXPECTED, //!< An unexpected state appeard somewhere (*NOOP*).
  ERROR_NOACTION,   //!< Any actions was registered since `being()` (*NOOP*).
  ERROR_TIMERPTR,   //!< The timer pointer was not defined properly (*NOOP*).
};

/*!
 * Normally the user doesn't need this values at all, they're here to construct
 * the `ps::PServo` object with these default values. But it can't be quite
 * helpful to know how this state machine works.
 *
 * @see ps::PServo
 */
namespace Default {
unsigned char constexpr MIN = 0;   //!< Minimal default degree position.
unsigned char constexpr MAX = 180; //!< Maximum default degree position.
unsigned char constexpr DELAY = 1; //!< Default delay between movement updates.
}; // namespace Default

/*!
 * List of all the private properties of `ps::PServo`. It's primary useful for
 * testing and monitoring strategies, but be aware that you cannot hack those
 * values inside the class, those are read only througth this struct.
 *
 * Usage example:
 * ```cpp
 * unsigned char active_action = myservo_machine.get_props().active_action;
 * unsigned char const actions_count  myservo_machine.get_props().actions_count;
 *
 * Serial.print("Current action: ");
 * Serial.print(active_action);
 *
 * if (active_action >= actions_count) {
 *   Serial.println(" (last one)");
 *
 * } else {
 *   Serial.println();
 * }
 * ```
 *
 * @see ps::State
 * @see ps::PServo
 */
typedef struct Props {
  State state;                 //!< Current state of the `ps::PServo` machine.
  unsigned long pc;            //!< Last registered process counter.
  unsigned long *const timer;  //!< Pointer to the timer variable in use.
  unsigned char const min;     //!< Minimal position that this machine can be.
  unsigned char const max;     //!< Maximum position that this machine can be.
  bool const is_resetable;     //!< Will the machine reset after it's halted?
  unsigned char curr_action;   //!< Says wich action it's trying to perform.
  unsigned char active_action; //!< Which action is, actually, performing.
  unsigned char actions_count; //!< How much actions was registred.
  unsigned char pos;           //!< Current servo position, will not be written.
  unsigned short delay;        //!< Delay stored for the current action move.
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
 *
 * @see ps::Props
 * @see ps::State
 */
class PServo {
public:
  /*!
   * The constructor need, at least, a pointer to a **timer** variable. This
   * variable, normally, is a global one, that multiple state machines can read
   * to calculate the delay asynchronously.
   *
   * For an example, at the top of your main Arduino sketch file, you may have a
   * `unsigned long timer = 0;` line, then, inside the `loop()` function, you'll
   * update that variable value everytime with the `millis()` function -- which
   * will return the time duration since your Arduino board was booted up, in
   * milisseconds.
   *
   * For an example:
   * ```cpp
   * unsigned long timer = 0;
   *
   * ps::PServo myservo_machine(&timer);
   *
   * void setup() {
   *   // Your setup code...
   * }
   *
   * void loop() {
   *   timer = millis();
   *
   *   // Your loop code...
   * }
   * ```
   *
   * This choice was made because every pointer in C++ uses 16 bits of memory
   * all the time, and an `unsigned long` uses 64 bits, which is much havier,
   * mainly when dealing with multiple object instances of this class.
   *
   * @param timer Pointer to a timer variable, normally related to the
   * `millis()` function.
   *
   * @see ps::PServo
   */
  PServo(unsigned long *const timer) : _timer(timer) {}

  /*!
   * The constructor need, at least, a pointer to a **timer** variable. This
   * variable, normally, is a global one, that multiple state machines can read
   * to calculate the delay asynchronously.
   *
   * You can also pass the reset setting, a *bolean* type parameter, to the
   * constructor. By default, the machine will not reset after the last action
   * completion.
   *
   * @param timer Pointer to a timer variable, normally related to the
   * `millis()` function.
   * @param is_resetable Configure the machine to reset after it's halted.
   */
  PServo(unsigned long *const timer, bool const is_resetable)
      : _timer(timer), _is_resetable(is_resetable) {}

  /*!
   * The constructor need, at least, a pointer to a **timer** variable. This
   * variable, normally, is a global one, that multiple state machines can read
   * to calculate the delay asynchronously.
   *
   * If needed, you can also configure the **max** and **min** position that
   * this machine can assume. It's quite useful when your building a robot, as
   * an example, that some movements can break the structure of the robot
   * phisically.
   *
   * @param timer Pointer to a timer variable, normally related to the
   * `millis()` function.
   * @param min Minimul position value that this machine can be.
   * @param max Maximum position value that this machine can be.
   *
   * @see ps::Default
   */
  PServo(unsigned long *const timer, unsigned char const min,
         unsigned char const max)
      : _timer(timer), _min(min), _max(max) {}

  /*!
   * The constructor need, at least, a pointer to a **timer** variable. This
   * variable, normally, is a global one, that multiple state machines can read
   * to calculate the delay asynchronously.
   *
   * And for more advanced configuration, or verbosity, you can combine the
   * other constructors into just this one. Where you pass the timer pointer,
   * the min-max values and the seeting to define if this machine should reset
   * to the first movement after done or not!
   *
   * @param timer Pointer to a timer variable, normally related to the
   * `millis()` function.
   * @param min Minimul position value that this machine can be.
   * @param max Maximum position value that this machine can be.
   * @param is_resetable Configure the machine to reset after it's halted.
   *
   * @see ps::Default
   */
  PServo(unsigned long *const timer, unsigned char const min,
         unsigned char const max, bool const is_resetable)
      : _timer(timer), _min(min), _max(max), _is_resetable(is_resetable) {}

  /*!
   * This function is the most important one, it should be used everytime at the
   * beginning of a move set collection, that's because it will handle most of
   * the possible states of this machine -- such as count the number of actions
   * it should take on the first call or check if the machine should halt or
   * start over again.
   *
   * The reason this function, and the others, returns a pointer reference of
   * the this same object class is to help you write sets of movements without
   * so much code repetition. It is a *syntax sugar*, if you will.
   *
   * For example, instead of doing this:
   * ```cpp
   * myservo_machine.begin();
   * myservo_machine.move(90, 10);
   * myservo_machine.move(180, 50);
   * myservo_machine.move(0, 30);
   * ```
   *
   * You can simply do:
   * ```cpp
   * myservo_machine.begin()
   *   ->move(90, 10)
   *   ->move(180, 50)
   *   ->move(0, 30);
   * ```
   *
   * @returns A pointer to this same object, allowing the use of the `->` syntax
   * to write a stream of actions that this state machine will perform.
   */
  PServo *begin(void);

  /*!
   * This function just calls it self again, but passing the
   * `ps::Default::DELAY` as a seccodary parameter to move (or, in this context,
   * update the value of the `_pos` private attribute) is a specific speed.
   *
   * This function overload can be helpful because, in some cases, is necessary
   * to just move the servo motors in an asynchronous way. So it helps to write
   * less code and be more descriptve of **what** you want to do and not
   * **how**.
   *
   * Check for more details the explaination of the full version of this
   * function call, the one that you need to specify the movement speed too.
   *
   * @param next_pos Next position that it needs to move to.
   *
   * @returns A pointer to this same object, allowing the use of the `->` syntax
   * to write a stream of actions that this state machine will perform.
   */
  PServo *move(unsigned char const next_pos);

  /*!
   * This method will start the **movement** action, it only works if the
   * machine is in the `ps::State::IN_ACTION`, otherwise, it will perform
   * nothing.
   *
   * > **Note**: In `ps::State::INITIALIZED` mode, it will, actually, count the
   * > movement. This is because the first run of the move set is for
   * > determining how much moves it should do before halt/reset.
   *
   * The `delay` paramter is not the overall duration of the movement! It is,
   * actually, the delay between each degree from the the current position to
   * the next one. For an example, if you're at `0` and want to go to `180` with
   * `10` for delay, the overall duration will be `1.8` seconds.
   *
   * Since this function is asynchronous, it **should be called every time in
   * the `loop()` function**! That's because it uses the `_timer` value --
   * defined in this class constructor -- to calculate how when the current
   * servo position should be updated.
   *
   * @param next_pos Next position that it needs to move to.
   * @param delay Delay between each position increment, until it's done.
   *
   * @returns A pointer to this same object, allowing the use of the `->` syntax
   * to write a stream of actions that this state machine will perform.
   */
  PServo *move(unsigned char const next_pos, unsigned short const delay);

  /*!
   * This method allows the user to inspect all the private attributes of the
   * object. It's quite useful for loggin or monitoring sketches, or maybe to
   * build more complex sketches that behaves differently depending of the state
   * of this object.
   *
   * @returns A `ps::Props` struct with all the private members values of the
   * instantiated object.
   */
  Props const get_props(void) const;

  /*!
   * Used to know which state the machine is currently in. Used to catch state
   * and handle state errors or build complex systems that behaves differently
   * based on the state of this machine.
   *
   * @returns The current state of the machine.
   */
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
 * @param s Valid state object of an `ps::PServo` state machine.
 *
 * Usage example:
 * ```cpp
 * ps::State const current_state = myservo_machine.get_state();
 *
 * Serial.print("Current state: ");
 * Serial.println(ps::state_text(current_state));
 * ```
 *
 * @returns A string representing the name of the specified state.
 *
 * @see ps::PServo
 * @see ps::State
 */
char const *state_text(State s);
}; // namespace ps
