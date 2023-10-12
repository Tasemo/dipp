#include <gtest/gtest.h>

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/pixel_cluster_mapping.hpp>
#include <thrill/api/concat_to_dia.hpp>
#include <thrill/api/gather.hpp>
#include <vector>

#include "../common.hpp"

TEST(PixelClusterMapping, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, 100, 100);
    model::KMeansModel model{};
    model.centers.emplace_back(0, 0, cv::Vec3b{});
    model.centers.emplace_back(100, 100, cv::Vec3b{});
    processing::PixelClusterMapping mapping(model);
    std::vector<model::Pixel> pixels;
    pixels.emplace_back(1, 1, cv::Vec3b{});
    pixels.emplace_back(2, 2, cv::Vec3b{});
    pixels.emplace_back(99, 99, cv::Vec3b{});
    auto dia = thrill::api::ConcatToDIA(ctx, pixels);
    auto result = mapping.process(context, dia).Gather(ctx.my_rank());
    auto compare = [](const std::vector<model::Pixel>& a, const std::vector<model::Pixel>& b) {
      return a.size() < b.size();
    };
    std::sort(result.begin(), result.end(), compare);
    EXPECT_EQ(2, result.size());
    EXPECT_EQ(1, result[0].size());
    EXPECT_EQ(2, result[1].size());
  };
  run_thrill(1, 1, start_func);
}
