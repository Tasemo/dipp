#include "paint_bounding_boxes.hpp"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <model/context.hpp>
#include <model/rect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <vector>

util::PaintBoundingBoxes::PaintBoundingBoxes(const model::Context& context, cv::Mat& image, const std::string& output_dir, const cv::Vec3b& color)
    : _context(context), _image(image.clone()), _output_dir(output_dir), _color(color) {
  std::filesystem::create_directories(output_dir);
}

void util::PaintBoundingBoxes::paint(const std::vector<model::Rect>& bounding_boxes) {
  for (int x = 0; x < _image.cols; x++) {
    for (int y = 0; y < _image.rows; y++) {
      size_t global_x = _context.to_global_x(static_cast<size_t>(x));
      size_t global_y = _context.to_global_y(static_cast<size_t>(y));
      bool on_edge = std::any_of(bounding_boxes.begin(), bounding_boxes.end(), [&global_x, &global_y](const model::Rect& box) {
        return box.is_on_edge(global_x, global_y);
      });
      if (on_edge) {
        _image.at<cv::Vec3b>(y, x) = cv::Vec3b{0, 255, 0};
      }
    }
  }
  cv::imwrite(_output_dir + std::to_string(_context.rank) + ".jpeg", _image);
}
