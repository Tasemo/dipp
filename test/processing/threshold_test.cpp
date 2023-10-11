#include <gtest/gtest.h>

#include <model/context.hpp>
#include <processing/threshold.hpp>
#include <thrill/api/gather.hpp>
#include <thrill/api/generate.hpp>

#include "../common.hpp"

TEST(Threshold, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers(), ctx.num_workers());
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&context](size_t index) {
      size_t global_x = context.index_to_global_x(index);
      size_t global_y = context.index_to_global_y(index);
      cv::Vec3b color = index % 2 == 0 ? cv::Vec3b{255, 255, 255} : cv::Vec3b{0, 0, 0};
      return model::Pixel(global_x, global_y, color);
    });
    auto threshold = processing::Threshold(254.0);
    auto result = threshold.process(context, pixels);
    auto gathered = result.Gather(ctx.my_rank());
    EXPECT_EQ(gathered.size(), context.global_width * context.global_height / 2 / ctx.num_workers());
    for (const auto& pixel : gathered) {
      auto expected = cv::Vec3b{255, 255, 255};
      EXPECT_EQ(pixel.bgr_value, expected);
    }
  };
  run_thrill(1, 4, start_func);
}
