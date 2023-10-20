#include <gtest/gtest.h>

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/estimate_clusters.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/equal_to_dia.hpp>
#include <thrill/api/size.hpp>
#include <vector>

#include "../common.hpp"

TEST(EstimateClusters, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, 3, 3);
    std::vector<model::Pixel> pixels;
    pixels.emplace_back(0, 0, cv::Vec3b{});
    pixels.emplace_back(2, 0, cv::Vec3b{});
    pixels.emplace_back(0, 2, cv::Vec3b{});
    pixels.emplace_back(2, 2, cv::Vec3b{});
    auto dia = thrill::api::EqualToDIA(ctx, pixels);
    processing::EstimateClusters estimate;
    auto result = estimate.process(context, dia);
    EXPECT_EQ(4, result.pixels.Size());
    EXPECT_EQ(4, result.cluster_count);
  };
  run_thrill(1, 1, start_func);
}

TEST(EstimateClusters, FullCluster) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, 2, 2);
    std::vector<model::Pixel> pixels;
    pixels.emplace_back(0, 0, cv::Vec3b{});
    pixels.emplace_back(1, 0, cv::Vec3b{});
    pixels.emplace_back(0, 1, cv::Vec3b{});
    pixels.emplace_back(1, 1, cv::Vec3b{});
    auto dia = thrill::api::EqualToDIA(ctx, pixels);
    processing::EstimateClusters estimate;
    auto result = estimate.process(context, dia);
    EXPECT_EQ(4, result.pixels.Size());
    EXPECT_EQ(1, result.cluster_count);
  };
  run_thrill(1, 1, start_func);
}
