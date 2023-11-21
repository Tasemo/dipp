#pragma once

#include <gtest/gtest.h>

#include <cassert>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <thrill/api/context.hpp>
#include <vector>

inline void run_thrill(size_t hosts, size_t worker_per_host, const std::function<void(thrill::Context&)>& job_startpoint) {
  thrill::api::MemoryConfig mem_config;
  mem_config.verbose_ = false;
  mem_config.enable_proc_profiler_ = false;
  mem_config.setup(100L * 1024 * 1024);
  thrill::api::RunLocalMock(mem_config, hosts, worker_per_host, job_startpoint);
}

inline std::vector<model::Pixel> generate_image(size_t width, size_t height, const cv::Vec3b& color, size_t offset = 0) {
  std::vector<model::Pixel> result;
  result.reserve(width * height);
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      result.emplace_back(x + offset, y + offset, color);
    }
  }
  assert(result.size() == width * height);
  return result;
}

inline cv::Mat assert_image(const std::string& image_path, size_t width, size_t height) {
  EXPECT_TRUE(std::filesystem::exists(image_path));
  auto image = cv::imread(image_path);
  EXPECT_EQ(image.cols, width);
  EXPECT_EQ(image.rows, height);
  return image;
}

inline cv::Mat assert_image(const std::string& image_path, size_t width, size_t height, const cv::Vec3b& expected) {
  auto image = assert_image(image_path, width, height);
  for (int x = 0; x < image.cols; x++) {
    for (int y = 0; y < image.rows; y++) {
      const auto& color = image.at<cv::Vec3b>(y, x);
      EXPECT_EQ(expected, color);
    }
  }
  return image;
}

class TemporaryFolder : public ::testing::Test {
 protected:
  std::string _temporary_folder;

  void SetUp() override {
    std::string folder = std::filesystem::temp_directory_path();
    _temporary_folder = folder + "/dipp/testing/";
    std::filesystem::remove_all(_temporary_folder);
    std::filesystem::create_directories(_temporary_folder);
  }
};
