#include <gtest/gtest.h>

TEST(GTest, should_pass) {
  unsigned char const x = 255;
  unsigned char const y = x;

  EXPECT_EQ(x, y);
}
