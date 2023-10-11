#pragma once

#include <model/context.hpp>
#include <model/rect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <string>
#include <vector>

namespace util {

/**
 * @brief colors each pixel in the original image if it is on the edge of some bounding box
 */
class PaintBoundingBoxes {
 private:
  const model::Context& _context;
  cv::Mat& _image;
  const std::string _output_dir;
  const cv::Vec3b _color;

 public:
  PaintBoundingBoxes(const model::Context& context, cv::Mat& image, const std::string& output_dir, const cv::Vec3b& color = cv::Vec3b{0, 255, 0});
  void paint(const std::vector<model::Rect>& bounding_boxes);
};

}  // namespace util
