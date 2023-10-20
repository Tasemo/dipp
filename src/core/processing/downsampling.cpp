#include "downsampling.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <thrill/api/dia.hpp>

processing::Downsampling::Downsampling(size_t factor)
    : _factor(factor) {}

thrill::DIA<model::Pixel> processing::Downsampling::process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const {
  return pixels.Filter([&](const model::Pixel& p) {
    return static_cast<size_t>(p.location[0] + p.location[1] + 1) % _factor == 0;
  });
}
