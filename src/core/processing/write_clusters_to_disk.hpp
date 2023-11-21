#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <model/rect.hpp>
#include <processing/pipeline.hpp>
#include <string>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief writes all clusters as images to disk
 *
 * @note while clusters do not have to be rectangular, images have to, so the image is padded with a color of your choice
 */
class WriteClustersToDisk : public Pipeline<thrill::DIA<std::vector<model::Pixel>>, thrill::DIA<std::vector<model::Pixel>>> {
 private:
  const std::string _output_folder;
  const cv::Vec3b _missing_color;
  mutable size_t _local_index{};

  std::vector<cv::Vec3b> get_image_data(const std::vector<model::Pixel>& cluster, size_t width, size_t height, size_t min_x, size_t min_y) const;

 public:
  explicit WriteClustersToDisk(const std::string& output_folder, const cv::Vec3b& missing_color = cv::Vec3b{255, 0, 255});
  thrill::DIA<std::vector<model::Pixel>> process(const model::Context& ctx, const thrill::DIA<std::vector<model::Pixel>>& clusters) const override;
};

}  // namespace processing
