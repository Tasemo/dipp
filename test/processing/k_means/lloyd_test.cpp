#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <model/context.hpp>
#include <model/k_means_init.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/k_means/lloyd.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/generate.hpp>

#include "../../common.hpp"

TEST(Lloyd, BasicTest) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&ctx](size_t) {
      auto location = cv::Vec2d{ctx.my_rank() * 4.0, ctx.my_rank() * 4.0};
      return model::Pixel{location, cv::Vec3b{}};
    });
    processing::Lloyd k_means(ctx.num_workers(), 10, 1.0, model::KMeansInit::RANDOM);
    auto model = k_means.process(context, pixels);
    auto compare = [](const model::Pixel& a, const model::Pixel& b) {
      return a.location[0] < b.location[0];
    };
    std::sort(model.centers.begin(), model.centers.end(), compare);
    EXPECT_EQ(model.centers.size(), ctx.num_workers());
    for (size_t i = 0; i < model.centers.size(); i++) {
      auto location = cv::Vec2d{i * 4.0, i * 4.0};
      EXPECT_EQ(model.centers[i].location, location);
    }
  };
  run_thrill(1, 2, start_func);
}

TEST(Lloyd, EqualValues) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [](size_t) {
      return model::Pixel{cv::Vec2d{}, cv::Vec3b{}};
    });
    processing::Lloyd k_means(ctx.num_workers(), 1, 1.0, model::KMeansInit::RANDOM);
    auto model = k_means.process(context, pixels);
    EXPECT_EQ(model.centers.size(), ctx.num_workers());
    for (const auto& center : model.centers) {
      EXPECT_EQ(center.location, cv::Vec2d{});
    }
  };
  run_thrill(1, 2, start_func);
}

TEST(Lloyd, Epsilon) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [](size_t) {
      return model::Pixel{cv::Vec2d{}, cv::Vec3b{}};
    });
    processing::Lloyd k_means(ctx.num_workers(), 10, 1.0, model::KMeansInit::RANDOM);
    auto model = k_means.process(context, pixels);
    EXPECT_EQ(model.iterations, 1);
  };
  run_thrill(1, 2, start_func);
}