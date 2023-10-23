#pragma once

#include <cstddef>
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
 private:
  size_t _min_cluster_size;

  size_t count_clusters(std::vector<std::vector<model::Pixel>>& grid) const;

 public:
  explicit EstimateClusters(size_t min_cluster_size = 0);
  ClusterEstimation process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
