#include <gtest/gtest.h>

#include <model/rect.hpp>

TEST(Rect, Contains) {
  model::Rect rect(1, 100, 1, 100);
  EXPECT_TRUE(rect.contains(1, 1));
  EXPECT_TRUE(rect.contains(100, 1));
  EXPECT_TRUE(rect.contains(100, 100));
  EXPECT_TRUE(rect.contains(1, 100));
  EXPECT_TRUE(rect.contains(50, 50));
  EXPECT_FALSE(rect.contains(101, 50));
  EXPECT_FALSE(rect.contains(0, 50));
  EXPECT_FALSE(rect.contains(50, 0));
  EXPECT_FALSE(rect.contains(50, 101));
}

TEST(Rect, IsOnEdge) {
  model::Rect rect(0, 100, 0, 100);
  EXPECT_TRUE(rect.is_on_edge(0, 0));
  EXPECT_TRUE(rect.is_on_edge(100, 0));
  EXPECT_TRUE(rect.is_on_edge(100, 100));
  EXPECT_TRUE(rect.is_on_edge(0, 100));
  EXPECT_FALSE(rect.is_on_edge(50, 50));
}
