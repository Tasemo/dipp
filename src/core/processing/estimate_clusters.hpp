#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

struct ClusterEstimation {
  thrill::DIA<model::Pixel> pixels;
  size_t local_cluster_count{};
  size_t global_cluster_count{};
};

class EstimateClusters : public Pipeline<thrill::DIA<model::Pixel>, ClusterEstimation> {
 public:
  ClusterEstimation process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
