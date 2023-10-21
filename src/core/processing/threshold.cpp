#include "threshold.hpp"

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>

processing::Threshold::Threshold(unsigned char cutoff, bool perceived)
    : _cutoff(cutoff), _perceived(perceived) {}

thrill::DIA<model::Pixel> processing::Threshold::process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const {
  return pixels.Filter([&](const model::Pixel& p) {
    unsigned char intensity = 0;
    if (_perceived) {
      intensity = 0.2126 * p.bgr_value[2] + 0.7152 * p.bgr_value[1] + 0.0722 * p.bgr_value[0];
    } else {
      intensity = (p.bgr_value[0] + p.bgr_value[1] + p.bgr_value[2]) / 3;
    }
    return intensity > _cutoff;
  });
}
