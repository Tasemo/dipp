#include <gtest/gtest.h>

#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/k_means/k_means_plus_plus.hpp>
#include <thrill/api/context.hpp>
#include <thrill/api/equal_to_dia.hpp>
#include <vector>

#include "../../common.hpp"

TEST(KMeansPlusPlus, BasicTest) {
  auto start_func = [](thrill::Context& ctx) {
    std::vector<model::Pixel> pixels;
    model::Pixel first(0, 0, cv::Vec3b{});
    model::Pixel last(100, 0, cv::Vec3b{});
    pixels.push_back(first);
    pixels.emplace_back(25, 0, cv::Vec3b{});
    pixels.emplace_back(75, 0, cv::Vec3b{});
    pixels.emplace_back(last);
    auto dia = thrill::EqualToDIA(ctx, pixels);
    processing::KMeansPlusPlus k_means;
    auto result = k_means.generate(2, dia);
    EXPECT_EQ(result.size(), 2);
    EXPECT_NE(result[0], result[1]);
    EXPECT_TRUE(result[1] == first || result[1] == last);
  };
  run_thrill(1, 2, start_func);
}
