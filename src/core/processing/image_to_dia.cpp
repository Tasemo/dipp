#include "image_to_dia.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <thrill/api/concat_to_dia.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/generate.hpp>

processing::ImageToDIA::ImageToDIA(const cv::Mat& image, bool streaming)
    : _image(image), _streaming(streaming) {}

thrill::DIA<model::Pixel> processing::ImageToDIA::process(const model::Context& ctx) const {
  if (_streaming) {
    return thrill::api::Generate(ctx.ctx, ctx.global_width * ctx.global_height, [&](size_t index) {
      size_t local_x = ctx.index_to_local_x(index);
      size_t local_y = ctx.index_to_local_y(index);
      size_t global_x = ctx.index_to_global_x(index);
      size_t global_y = ctx.index_to_global_y(index);
      return model::Pixel{global_x, global_y, _image.at<cv::Vec3b>(local_y, local_x)};
    });
  } else {
    std::vector<model::Pixel> pixels;
    pixels.reserve(ctx.local_width * ctx.local_height);
    for (size_t i = 0; i < ctx.local_width * ctx.local_height; i++) {
      size_t local_x = i % ctx.local_width;
      size_t local_y = i / ctx.local_width;
      size_t global_x = ctx.to_global_x(local_x);
      size_t global_y = ctx.to_global_y(local_y);
      pixels.emplace_back(global_x, global_y, _image.at<cv::Vec3b>(local_y, local_x));
    }
    return thrill::api::ConcatToDIA(ctx.ctx, std::move(pixels));
  }
}
