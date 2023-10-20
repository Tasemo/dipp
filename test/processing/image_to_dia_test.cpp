#include <gtest/gtest.h>
#include <opencv2/core/hal/interface.h>

#include <cmath>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/image_to_dia.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/gather.hpp>
#include <vector>

#include "../common.hpp"

TEST(ImageToDIA, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, std::sqrt(ctx.num_workers()), std::sqrt(ctx.num_workers()));
    cv::Mat image(1, 1, CV_8UC3, cv::Vec3b{});
    processing::ImageToDIA image_to_dia(image);
    auto result = image_to_dia.process(context);
    auto gathered = result.Gather(ctx.my_rank());
    EXPECT_EQ(1, gathered.size());
    model::Pixel expected(context.x_tile, context.y_tile, cv::Vec3b{});
    EXPECT_EQ(expected, gathered[0]);
  };
  run_thrill(1, 4, start_func);
}
