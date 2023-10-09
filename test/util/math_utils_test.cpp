#include <gtest/gtest.h>

#include <util/math_utils.hpp>

TEST(MathUtils, BasicTest) {
  EXPECT_EQ(0.0, util::map_range(0.0, 1000.0, 0, 100, 0));
  EXPECT_EQ(1000.0, util::map_range(0.0, 1000.0, 0, 100, 100));
  EXPECT_EQ(500.0, util::map_range(0.0, 1000.0, 0, 100, 50));
}
