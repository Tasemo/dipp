#pragma once

#include <model/pixel.hpp>
#include <opencv2/imgcodecs.hpp>
#include <processing/pipeline.hpp>
#include <string>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

class WriteImageToDisk : public Pipeline<thrill::DIA<model::Pixel>, thrill::DIA<model::Pixel>> {
 private:
  const std::string _output_folder;
  const cv::Vec3b _missing_color;

 public:
  explicit WriteImageToDisk(const std::string& output_folder, const cv::Vec3b& missing_color = cv::Vec3b{255, 0, 255});
  thrill::DIA<model::Pixel> process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
