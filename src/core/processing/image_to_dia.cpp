#include "image_to_dia.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/generate.hpp>

processing::ImageToDIA::ImageToDIA(const cv::Mat& image)
    : _image(image) {}

thrill::DIA<model::Pixel> processing::ImageToDIA::process(const model::Context& ctx) const {
  return thrill::api::Generate(ctx.ctx, ctx.global_width * ctx.global_height, [&](size_t index) {
    size_t local_x = ctx.index_to_local_x(index);
    size_t local_y = ctx.index_to_local_y(index);
    size_t global_x = ctx.index_to_global_x(index);
    size_t global_y = ctx.index_to_global_y(index);
    return model::Pixel{global_x, global_y, _image.at<cv::Vec3b>(local_y, local_x)};
  });
}
