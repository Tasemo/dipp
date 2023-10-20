#include <gtest/gtest.h>

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <model/rect.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/validation.hpp>
#include <thrill/api/concat_to_dia.hpp>
#include <thrill/api/context.hpp>
#include <vector>

#include "../common.hpp"

TEST(Validation, BasicTest) {
  auto start_func = [&](thrill::Context &ctx) {
    model::Context context(ctx, 100, 100);
    std::vector<model::Rect> bounding_boxes;
    bounding_boxes.emplace_back(0, 100, 0, 100);
    bounding_boxes.emplace_back(100, 200, 0, 100);
    std::vector<std::vector<model::Pixel>> clusters;
    clusters.emplace_back(1, model::Pixel(50, 50, cv::Vec3b{}));
    clusters.emplace_back(1, model::Pixel(150, 50, cv::Vec3b{}));
    clusters.emplace_back(1, model::Pixel(160, 50, cv::Vec3b{}));
    auto dia = thrill::api::ConcatToDIA(ctx, clusters);
    processing::Validation validation(bounding_boxes);
    auto result = validation.process(context, dia);
    EXPECT_EQ(2, result.total_clusters);
    EXPECT_EQ(1, result.uncut_clusters);
    EXPECT_EQ(1, result.cut_clusters);
    EXPECT_EQ(0, result.not_found_clusters);
  };
  run_thrill(1, 1, start_func);
}
