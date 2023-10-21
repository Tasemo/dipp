#pragma once

#include <cstddef>
#include <memory>
#include <model/context.hpp>
#include <model/k_means_init.hpp>
#include <model/k_means_model.hpp>
#include <processing/estimate_clusters.hpp>
#include <processing/k_means/k_means_init.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

class Lloyd : public Pipeline<thrill::DIA<model::Pixel>, model::KMeansModel>,
              public Pipeline<ClusterEstimation, model::KMeansModel> {
 private:
  size_t _cluster_count;
  size_t _max_iteratations;
  double _epsilon;
  std::unique_ptr<KMeansInit> _init;

  bool epsilon_reached(const model::KMeansModel& model, const std::vector<model::Pixel>& new_centers) const;
  model::KMeansModel process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels, size_t cluster_count) const;

 public:
  Lloyd(size_t cluster_count, size_t max_iteratations, size_t epsilon, model::KMeansInit init);
  model::KMeansModel process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
  model::KMeansModel process(const model::Context& ctx, const ClusterEstimation& estimation) const override;
};

}  // namespace processing
