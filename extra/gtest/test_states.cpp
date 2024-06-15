#include <gtest/gtest.h>

#include "../../src/PServo.h"

TEST(State, should_be_in_error_state_when_no_movements) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);

  pservo.begin();
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.begin();
  ASSERT_EQ(pservo.get_state(), State::ERROR_NOACTION);
}

TEST(State, should_initialize_then_start_the_first_action) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);

  // First loop routine from the Arduino `loop()` function.
  pservo.begin();
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.move(10, 15);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.move(20, 15);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.move(30, 15);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  // Second loop routine, it should update to the next state.
  pservo.begin();
  ASSERT_EQ(pservo.get_state(), State::IN_ACTION);

  pservo.move(30, 15);
  ASSERT_EQ(pservo.get_state(), State::IN_ACTION);

  // Keep in action mode, only update when all movements are done.
  pservo.begin();
  ASSERT_EQ(pservo.get_state(), State::IN_ACTION);

  pservo.move(30, 15);
  ASSERT_EQ(pservo.get_state(), State::IN_ACTION);
}
