#include <gtest/gtest.h>

#include <cstddef>
#include <model/context.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/downsampling.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/generate.hpp>
#include <thrill/api/size.hpp>

#include "../common.hpp"

TEST(Downsampling, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers(), ctx.num_workers());
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&context](size_t index) {
      size_t global_x = context.index_to_global_x(index);
      size_t global_y = context.index_to_global_y(index);
      return model::Pixel(global_x, global_y, cv::Vec3b{});
    });
    auto downsampling = processing::Downsampling(2);
    auto result = downsampling.process(context, pixels);
    EXPECT_EQ(result.Size(), context.global_width * context.global_height / 2);
  };
  run_thrill(1, 4, start_func);
}
