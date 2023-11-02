#include "processing/k_means/k_means.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <model/context.hpp>
#include <model/distribution.hpp>
#include <model/k_means_init.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/generate.hpp>

#include "../../common.hpp"

TEST(KMeans, BasicTest) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&ctx](size_t) {
      auto location = cv::Vec2d{ctx.my_rank() * 4.0, ctx.my_rank() * 4.0};
      return model::Pixel{location, cv::Vec3b{}};
    });
    processing::KMeans k_means(ctx.num_workers(), 10, 1.0, model::KMeansInit::RANDOM, model::Distribution::LLOYD);
    auto model = k_means.process(context, pixels);
    EXPECT_EQ(model.centers.size(), ctx.num_workers());
  };
  run_thrill(1, 2, start_func);
}

TEST(KMeans, Epsilon) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [](size_t) {
      return model::Pixel{cv::Vec2d{}, cv::Vec3b{}};
    });
    processing::KMeans k_means(ctx.num_workers(), 10, 1.0, model::KMeansInit::RANDOM, model::Distribution::LLOYD);
    auto model = k_means.process(context, pixels);
    EXPECT_EQ(model.iterations, 1);
  };
  run_thrill(1, 2, start_func);
}
