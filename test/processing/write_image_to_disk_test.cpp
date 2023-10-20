#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <processing/write_image_to_disk.hpp>
#include <string>
#include <thrill/api/context.hpp>
#include <thrill/api/generate.hpp>

#include "../common.hpp"

using WriteImageToDisk = TemporaryFolder;

TEST_F(WriteImageToDisk, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers(), ctx.num_workers());
    auto expected = cv::Vec3b{128, 128, 128};
    auto pixels = thrill::api::Generate(ctx, context.global_width * context.global_height, [&context, &expected](size_t index) {
      size_t global_x = context.index_to_global_x(index);
      size_t global_y = context.index_to_global_y(index);
      return model::Pixel(global_x, global_y, expected);
    });
    auto write_to_test = processing::WriteImageToDisk(_temporary_folder);
    write_to_test.process(context, pixels);
    ASSERT_TRUE(std::filesystem::exists(_temporary_folder + std::to_string(ctx.my_rank()) + ".jpeg"));
    auto image = cv::imread(_temporary_folder + std::to_string(ctx.my_rank()) + ".jpeg");
    for (int x = 0; x < image.cols; x++) {
      for (int y = 0; y < image.rows; y++) {
        cv::Vec3b color = image.at<cv::Vec3b>(y, x);
        EXPECT_EQ(expected, color);
      }
    }
  };
  run_thrill(1, 4, start_func);
}
