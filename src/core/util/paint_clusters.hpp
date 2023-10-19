#pragma once

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <opencv2/core/mat.hpp>
#include <string>

namespace util {

/**
 * @brief colors each pixel in the original image with a unique color to match the cluster it belongs to
 */
class PaintClusters {
 private:
  const model::Context& _context;
  cv::Mat _image;
  const std::string _output_dir;

 public:
  PaintClusters(const model::Context& context, cv::Mat& image, const std::string& output_dir);
  void paint(const model::KMeansModel& model);
};

}  // namespace util
