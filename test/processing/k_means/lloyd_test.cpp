#include <gtest/gtest.h>

#include <cstddef>
#include <model/context.hpp>
#include <model/k_means_init.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/k_means/lloyd.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/generate.hpp>
#include <vector>

#include "../../common.hpp"

TEST(Lloyd, BasicTest) {
  auto start_func = [](thrill::Context& ctx) {
    model::Context context(ctx, 10, 10);
    std::vector<model::Pixel> centers;
    for (size_t i = 0; i < ctx.num_workers(); i++) {
      auto location = cv::Vec2d{i * 4.0, i * 4.0};
      centers.emplace_back(location, cv::Vec3b{});
    }
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&](size_t) {
      return centers[context.rank];
    });
    model::KMeansModel model{0, centers};
    processing::Lloyd lloyd;
    auto result = lloyd.perform(context, model, pixels);
    EXPECT_EQ(result.size(), ctx.num_workers());
    for (size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], centers[i]);
    }
  };
  run_thrill(1, 2, start_func);
}
