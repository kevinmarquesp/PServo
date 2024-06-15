#include <gtest/gtest.h>

#include "../../src/PServo.h"

TEST(Defaults, should_return_default_constructor_props) {
  using namespace ps;

  unsigned long timer = 0;

  PServo pservo(&timer);
  State const s = pservo.get_state();
  Props const p = pservo.get_props();

  EXPECT_EQ(s, State::STANDBY);
  EXPECT_EQ(p.pc, 0);
  EXPECT_EQ(p.min, Default::MIN);
  EXPECT_EQ(p.max, Default::MAX);
  EXPECT_FALSE(p.is_resetable);
  EXPECT_EQ(p.curr_action, 0);
  EXPECT_EQ(p.actions_count, 0);
  EXPECT_EQ(p.pos, 0);
}

TEST(Defaults, should_asign_custom_restable) {
  using namespace ps;

  unsigned long timer = 0;
  bool const is_resetable = true;

  PServo pservo(&timer, is_resetable);
  Props const p = pservo.get_props();

  EXPECT_EQ(p.min, Default::MIN);
  EXPECT_EQ(p.max, Default::MAX);
  EXPECT_TRUE(p.is_resetable);
}

TEST(Defaults, should_asign_custom_min_and_max) {
  using namespace ps;

  unsigned long timer = 0;
  unsigned char const custom_min = 25;
  unsigned char const custom_max = 120;

  PServo pservo(&timer, custom_min, custom_max);
  Props const p = pservo.get_props();

  EXPECT_EQ(p.min, custom_min);
  EXPECT_EQ(p.max, custom_max);
  EXPECT_FALSE(p.is_resetable);
}

TEST(Defaults, should_asign_custom_min_max_and_restable) {
  using namespace ps;

  unsigned long timer = 0;
  unsigned char const custom_min = 5;
  unsigned char const custom_max = 185; // Numbers > 180 is also valid.
  bool const is_resetable = true;

  PServo pservo(&timer, custom_min, custom_max, is_resetable);
  Props const p = pservo.get_props();

  EXPECT_EQ(p.min, custom_min);
  EXPECT_EQ(p.max, custom_max);
  EXPECT_TRUE(p.is_resetable);
}
