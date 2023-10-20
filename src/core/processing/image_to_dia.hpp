#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

/**
 * @brief a producer that creates a DIA from an image
 */
class ImageToDIA : public Producer<thrill::DIA<model::Pixel>> {
 private:
  const cv::Mat& _image;

 public:
  explicit ImageToDIA(const cv::Mat& image);
  thrill::DIA<model::Pixel> process(const model::Context& ctx) const override;
};

}  // namespace processing
