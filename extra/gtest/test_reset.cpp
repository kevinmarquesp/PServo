#include <gtest/gtest.h>

#include "../../src/PServo.h"

TEST(Reset, should_reset_to_the_initial_state) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);

  while (true) {
    pservo.begin()->move(180, 10);

    if (pservo.is_state(State::HALT))
      break;

    ++timer;
  }

  ASSERT_EQ(pservo.props().state, State::HALT);
  ASSERT_EQ(pservo.props().active_action, 1);
  ASSERT_EQ(pservo.props().actions_count, 1);

  pservo.reset();

  ASSERT_EQ(pservo.props().state, State::STANDBY);
  ASSERT_EQ(pservo.props().active_action, 0);
  ASSERT_EQ(pservo.props().actions_count, 0);
}
