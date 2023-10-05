#include <gtest/gtest.h>

#include <util/color_utils.hpp>

TEST(ColorUtils, BasicTest) {
  auto result = util::generate_colors(7);
  EXPECT_EQ(result.size(), 7);
  EXPECT_EQ(result[0], (cv::Vec3b{0, 0, 0}));
  EXPECT_EQ(result[6], (cv::Vec3b{255, 255, 255}));
}

TEST(ColorUtils, MinimalSpace) {
  auto result = util::generate_colors(8, 255);
  EXPECT_EQ(result.size(), 8);
  EXPECT_EQ(result[0], (cv::Vec3b{0, 0, 0}));
  EXPECT_EQ(result[7], (cv::Vec3b{0, 0, 0}));
}
