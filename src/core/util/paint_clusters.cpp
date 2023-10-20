#include "paint_clusters.hpp"

#include <cstddef>
#include <filesystem>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <util/color_utils.hpp>

util::PaintClusters::PaintClusters(const model::Context& context, cv::Mat& image, const std::string& output_dir)
    : _context(context), _image(image.clone()), _output_dir(output_dir) {
  std::filesystem::create_directories(output_dir);
}

void util::PaintClusters::paint(const model::KMeansModel& model) {
  auto colors = util::generate_colors(model.centers.size());
  for (int x = 0; x < _image.cols; x++) {
    for (int y = 0; y < _image.rows; y++) {
      size_t global_x = _context.to_global_x(static_cast<size_t>(x));
      size_t global_y = _context.to_global_y(static_cast<size_t>(y));
      auto& color = _image.at<cv::Vec3b>(y, x);
      auto pixel = model::Pixel(global_x, global_y, color);
      size_t cluster_id = model.nearest_cluster(pixel);
      color = colors[cluster_id];
    }
  }
  cv::imwrite(_output_dir + std::to_string(_context.rank) + ".jpeg", _image);
}
