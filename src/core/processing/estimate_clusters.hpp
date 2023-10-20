#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

struct Estimation {
  thrill::DIA<model::Pixel> pixels;
  size_t cluster_count{};
};

class EstimateClusters : public Pipeline<thrill::DIA<model::Pixel>, Estimation> {
 public:
  Estimation process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
