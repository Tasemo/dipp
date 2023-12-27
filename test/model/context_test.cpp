#include <gtest/gtest.h>

#include <model/context.hpp>

#include "../common.hpp"

TEST(Context, BasicTest) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 20, 10);
    EXPECT_EQ(context.global_width, 20);
    EXPECT_EQ(context.global_height, 10);
    EXPECT_EQ(context.local_width, 10);
    EXPECT_EQ(context.local_height, 5);
    EXPECT_EQ(context.x_tiles, 2);
    EXPECT_EQ(context.y_tiles, 2);
    EXPECT_EQ(context.index_to_local_x(199), 9);
    EXPECT_EQ(context.index_to_local_y(199), 4);
    EXPECT_EQ(context.to_local_x(19), 9);
    EXPECT_EQ(context.to_local_y(9), 4);
  };
  run_thrill(1, 4, start_func);
}
