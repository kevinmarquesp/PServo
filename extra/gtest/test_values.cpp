#include <gtest/gtest.h>

#include "../../src/PServo.h"

TEST(Values, should_count_the_actions_on_the_first_run) {
  using namespace ps;

  unsigned long timer = 0;
  unsigned char const ACTIONS_COUNT = 5;

  PServo pservo(&timer, 0, 180, false);

  pservo.begin();

  for (unsigned char i = 1; i <= ACTIONS_COUNT; ++i) {
    pservo.move(i * 10, 3);
    ASSERT_EQ(pservo.get_props().actions_count, i);
  }
}

TEST(Values, should_preserve_the_actions_count_after_initialization) {
  using namespace ps;

  unsigned long timer = 0;
  unsigned char const ACTIONS_COUNT = 5;

  PServo pservo(&timer, 0, 180, false);

  pservo.begin();

  for (unsigned char i = 1; i <= ACTIONS_COUNT; ++i)
    pservo.move(i * 10, 3);

  pservo.begin();
  ASSERT_EQ(pservo.get_props().actions_count, ACTIONS_COUNT);

  for (unsigned char i = 1; i <= ACTIONS_COUNT; ++i)
    pservo.move(i * 10, 3);

  ASSERT_EQ(pservo.get_props().actions_count, ACTIONS_COUNT);
}

TEST(Values, should_keep_updating_the_curr_action_position) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);

  pservo.begin();
  pservo.move(5, 3);
  pservo.move(10, 3);
  pservo.move(15, 3);

  pservo.begin();
  ASSERT_EQ(pservo.get_props().curr_action, 0);
  ASSERT_EQ(pservo.get_props().active_action, 0);
  ASSERT_EQ(pservo.get_props().actions_count, 3);

  pservo.move(5, 3);
  ASSERT_EQ(pservo.get_props().curr_action, 1); // Current action after update.
  ASSERT_EQ(pservo.get_props().active_action, 0);
  ASSERT_EQ(pservo.get_props().actions_count, 3);

  pservo.move(10, 3);
  ASSERT_EQ(pservo.get_props().curr_action, 2);
  ASSERT_EQ(pservo.get_props().active_action, 0);
  ASSERT_EQ(pservo.get_props().actions_count, 3);

  pservo.move(15, 3);
  ASSERT_EQ(pservo.get_props().curr_action, 3);
  ASSERT_EQ(pservo.get_props().active_action, 0);
  ASSERT_EQ(pservo.get_props().actions_count, 3);
}

TEST(Values, should_update_active_action_after_the_previous_was_completed) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);

  pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);

  for (unsigned char i = 0; i < 6; ++i) { // From 0deg to 5deg! The 6th should
                                          // start the next (1) action.
    timer += 4;

    pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);
  }

  ASSERT_EQ(pservo.get_props().active_action, 1);

  for (unsigned char i = 0; i < 6; ++i) { // From 5deg to 10deg! The 6th should
                                          // start the next (1) action.
    timer += 4;

    pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);
  }

  ASSERT_EQ(pservo.get_props().active_action, 2);

  for (unsigned char i = 0; i < 6; ++i) { // From 10deg to 15deg! The 6th should
                                          // mark the state as State::DONE.
    timer += 4;

    pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);
  }

  ASSERT_EQ(pservo.get_props().active_action, 2);

  // No matter how I try, it can't keep adding 1 to the active state variable.

  pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);
  ASSERT_EQ(pservo.get_props().active_action, 3);

  pservo.begin()->move(5, 3)->move(10, 3)->move(15, 3);
  ASSERT_EQ(pservo.get_props().active_action, 3);
}

/*******************************************************************************
TEST(Values, should_halt_the_machine_after_the_last_movement) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, false);
}

TEST(Values, should_reset_to_initialized_state_if_marked_as_reset) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer, 0, 180, true);
}
*******************************************************************************/
