#pragma once

#include <model/pixel.hpp>
#include <processing/k_means/init/init_strategy.hpp>

namespace processing {

/**
 * @brief implementation of a random partition (Forgy) init strategy
 *
 * @see Forgy, "Cluster analysis of multivariate data: efficiency versus interpretability of classifications", 1965
 */
class KMeansRandomPartition : public InitStrategy {
 public:
  std::vector<model::Pixel> generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
