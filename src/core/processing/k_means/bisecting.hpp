#pragma once

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/k_means/init/random.hpp>
#include <processing/k_means/k_means.hpp>
#include <processing/k_means/k_means_strategy.hpp>
#include <processing/k_means/lloyd.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief implementation of the bisecting k-Means algorithm
 */
class Bisecting : public KMeansStrategy {
 private:
  size_t get_initial_cluster_count(size_t requested) const override;

 public:
  std::vector<model::Pixel> perform(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
