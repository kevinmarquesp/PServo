#include <gtest/gtest.h>

#include "../../src/PServo.h"

TEST(MValues, should_move_from_standby_to_initialized_then_halted_and_moving) {
  using namespace ps;

  PServo pservo;

  pservo.begin(0);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.move(90);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.move(180);
  ASSERT_EQ(pservo.get_state(), State::INITIALIZED);

  pservo.halt();
  ASSERT_EQ(pservo.get_state(), State::HALTED);

  pservo.begin(0);
  ASSERT_EQ(pservo.get_state(), State::MOVING);
}

TEST(MValues, should_count_the_actions_when_initialized) {
  using namespace ps;

  PServo pservo;
  unsigned char const __MAX_MOVES = 5;

  pservo.begin(0);

  for (unsigned char i = 1; i <= __MAX_MOVES; ++i) {
    pservo.move(180);
    ASSERT_EQ(pservo.get_props().actions_count, i);
  }

  ASSERT_EQ(pservo.get_props().actions_count, __MAX_MOVES);
  pservo.halt();
}
