#include <gtest/gtest.h>

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/pixel_worker_mapping.hpp>
#include <thrill/api/gather.hpp>
#include <thrill/api/generate.hpp>

#include "../common.hpp"

TEST(PixelWorkerMapping, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers(), ctx.num_workers());
    model::Pixel expected(ctx.my_rank(), ctx.my_rank(), cv::Vec3b{});
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&expected](size_t) {
      return expected;
    });
    processing::PixelWorkerMapping mapping;
    auto result = mapping.process(context, pixels).Gather(ctx.my_rank());
    ASSERT_EQ(1, result.size());
    for (const auto& pixel : result[0]) {
      EXPECT_EQ(expected, pixel);
    }
  };
  run_thrill(1, 4, start_func);
}
