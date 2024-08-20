# PServo

An Arduino library designed to make working with multiple servo motors simpler
and more intuitive, especially for those building complex robots. With PServo,
you can easily control the movements of multiple servos at the same time and
even create more advanced motion sequences. The library introduces a design
pattern that I call Scene-Oriented Programming, making it easier to choreograph
and synchronize servo movements for your projects.

![PServo demo](demo.gif)


## Installation

PServo is built as a *state machine*, so it requires the `Servo.h` library to
function properly. Make sure you have that installed as well.


### Option 1: Manual Installation

+   Visit the [releases page](https://github.com/kevinmarquesp/PServo/releases)
    on GitHub.
+   Download the `.zip` file for the version you want.
+   Extract the `.zip` file into your Arduino libraries folder:
    +   On Linux or Mac: `~/Arduino/libraries`
    +   On Windows: `Documents\Arduino\libraries`


### Option 2: Using Arduino CLI (Recommended)

If you prefer a faser method, you can use the following [Arduino CLI](https://arduino.github.io/arduino-cli/1.0/)
command:

```bash
arduino-cli lib install --git-url https://github.com/kevinmarquesp/PServo
```

> [!NOTE]
> If you haven’t configured the Arduino CLI to allow unsafe library
> installations, you might need to follow [this documentation](https://arduino.github.io/arduino-cli/0.35/configuration/#configuration-keys)
> to enable it. In short, run `arduino-cli config init`, open the generated
> file, and set `library.enable_unsafe_install` to `true`. That’s it!


## Usage

This library doesn’t use the `delay()` function to control the speed of
movements, as it would pause the entire program, which isn’t suitable for most
Arduino applications. Instead, it relies on the `millis()` function to determine
when to wait and when to execute movements, similar to other Arduino projects.

The general structure of your application should look something like this:

```cpp
unsigned long timer = 0;

void setup(void) {
  // ...rest of the code...
}

void loop(void) {
  timer = millis();

  // ...rest of the code...
}
```

First, import the `Servo.h` and `PServo.h` libraries, then create the servo
objects you want to control, along with a PServo object for each servo. The
PServo object will calculate the servo position, and you'll need to update the
actual servo object with these calculated values.

```cpp
#include <Servo.h>
#include <PServo.h>

unsigned long timer = 0;

Servo servo_right;
Servo servo_left;

PServo machine_right(&timer, 0, 180, true);
PServo machine_left(&timer, 0, 180, true);

void setup(void) {
  servo_right.attach(7);
  servo_left.attach(6);
}
```

> In this example, the line `PServo machine_left(&timer, 0, 180, true);`
> initializes a PServo object with a minimum value of 0, a maximum value of 180,
> and the `true` parameter indicates that this machine allows looping. When the
> movement sequence ends, it will automatically restart.

Since the `timer` pointer lets the PServo object track the current time in your
application, you'll need to update the servo positions within the `loop()`
function. The process involves getting the current position from each PServo
object and then writing that position to the corresponding servo object:

```cpp
void loop(void) {
    timer = millis();

    servo_right.write(machine_right.get_pos());
    servo_left.write(machine_left.get_pos());

    // ...rest of the code...
}
```

To configure the moveset for the servos, start with the `.begin()` method,
followed by a series of `move()` calls. The `.begin()` method indicates the start
of a new movement sequence, while each `move()` method specifies a movement to
be performed. The PServo object will manage which movement is currently active
and which should be skipped. It’s important to define this moveset within the
`loop()` function:

```cpp
void loop(void) {
    // ...rest of the code...

    machine_right.begin();
    machine_right.move(180, 15);  // Go to 180º waiting 15 ms for each deg update.
    machine_right.move(0, 20);    // Go to 0º waiting 0 ms for each deg update.

    machine_left.begin();
    machine_left.move(180, 20);
    machine_left.move(0, 0);
}
```

Since repeatedly writing the machine name can be tedious, this library provides
a syntax shortcut to simplify your code. The following code does the same as the
one above:

```cpp
void loop(void) {
    // ...rest of the code...

    machine_right.begin()
        ->move(180, 15)
        ->move(0, 20);

    machine_left.begin()
        ->move(180, 20)
        ->move(0, 0);
}
```

This is just a basic example. For more details on what this library can do and
how to implement additional features, check out the [official documentation](https://kevinmarquesp.github.io/PServo/)
and the [example sketches](https://github.com/kevinmarquesp/PServo/tree/main/examples)
to learn more.


## Contributing

If you’d like to contribute to the development of PServo, follow these steps:

### Getting Started

1.  Clone the repository:

```bash
git clone https://github.com/kevinmarquesp/PServo
cd PServo
```

2.  Set up dependencies:

```bash
make deps # Clone and/or build vendorized libraries.
```


### Testing (Arduino Board not Required)

You can run tests without needing an Arduino board.


#### Run All Tests

To run the entire test suite:

```bash
make deps
make test
```


#### Run a Specific Test

To run a specific test, specify the test file with the `GTEST_UNITS` variable:

```bash
make deps
make test GTEST_UNITS=extra/gtest/test_gtest.cpp
```
