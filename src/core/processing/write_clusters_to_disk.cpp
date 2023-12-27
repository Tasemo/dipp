#include "write_clusters_to_disk.hpp"

#include <opencv2/core/hal/interface.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <limits>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <model/rect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

model::Rect get_image_bounds(const std::vector<model::Pixel>& cluster) {
  auto max_value = std::numeric_limits<size_t>::max();
  size_t min_x = max_value, max_x = 0, min_y = max_value, max_y = 0;
  for (const auto& pixel : cluster) {
    min_x = std::min(min_x, static_cast<size_t>(pixel.location[0]));
    max_x = std::max(max_x, static_cast<size_t>(pixel.location[0]));
    min_y = std::min(min_y, static_cast<size_t>(pixel.location[1]));
    max_y = std::max(max_y, static_cast<size_t>(pixel.location[1]));
  }
  return {min_x, max_x, min_y, max_y};
}

std::vector<cv::Vec3b> processing::WriteClustersToDisk::get_image_data(const std::vector<model::Pixel>& cluster, size_t width, size_t height, size_t min_x, size_t min_y) const {
  std::vector<cv::Vec3b> pixels(width * height, _missing_color);
  for (const auto& pixel : cluster) {
    size_t local_x = static_cast<size_t>(pixel.location[0] - min_x) % width;
    size_t local_y = static_cast<size_t>(pixel.location[1] - min_y) % width;
    size_t index = local_y * width + local_x;
    pixels.at(index) = pixel.bgr_value;
  }
  assert(pixels.size() == width * height);
  return pixels;
}

processing::WriteClustersToDisk::WriteClustersToDisk(const std::string& output_folder, const cv::Vec3b& missing_color)
    : _output_folder(output_folder), _missing_color(missing_color) {
  std::filesystem::create_directories(output_folder);
}

thrill::DIA<std::vector<model::Pixel>> processing::WriteClustersToDisk::process(const model::Context& ctx, const thrill::DIA<std::vector<model::Pixel>>& clusters) const {
  if (ctx.no_debug) {
    return clusters;
  }
  _local_index = 0;
  auto mapped = clusters.Map([&](const std::vector<model::Pixel>& cluster) {
    auto bounds = get_image_bounds(cluster);
    auto width = bounds.max_x - bounds.min_x + 1;
    auto height = bounds.max_y - bounds.min_y + 1;
    auto image_data = get_image_data(cluster, width, height, bounds.min_x, bounds.min_y);
    cv::Mat image(height, width, CV_8UC3, image_data.data());
    auto image_path = _output_folder + std::to_string(ctx.rank) + '-' + std::to_string(_local_index++) + ".jpeg";
    cv::imwrite(image_path, image);
    return cluster;
  });
  return mapped.Collapse();
}
