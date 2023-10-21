#include "write_image_to_disk.hpp"

#include <opencv2/core/hal/interface.h>

#include <cstddef>
#include <filesystem>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

processing::WriteImageToDisk::WriteImageToDisk(const std::string& output_folder, const cv::Vec3b& missing_color)
    : _output_folder(output_folder), _missing_color(missing_color) {
  std::filesystem::create_directories(output_folder);
}

thrill::DIA<model::Pixel> processing::WriteImageToDisk::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  std::vector<cv::Vec3b> local(ctx.local_width * ctx.local_height, _missing_color);
  auto to_local = pixels.Map([&ctx, &local](const model::Pixel& p) {
    size_t local_x = ctx.to_local_x(p.location[0]);
    size_t local_y = ctx.to_local_y(p.location[1]);
    size_t index = local_y * ctx.local_height + local_x;
    local.at(index) = p.bgr_value;
    return p;
  });
  auto cached = to_local.Cache().Execute();
  cv::Mat image(ctx.local_height, ctx.local_width, CV_8UC3, local.data());
  cv::imwrite(_output_folder + std::to_string(ctx.rank) + ".jpeg", image);
  return cached;
}
