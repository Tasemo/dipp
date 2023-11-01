#pragma once

#include <processing/k_means/init/init_strategy.hpp>

namespace processing {

/**
 * @brief implementation of a random (MacQueen) init strategy
 *
 * @see MacQueen, "Some Methods for classification and Analysis of Multivariate Observations", 1967
 */
class KMeansRandom : public InitStrategy {
 public:
  std::vector<model::Pixel> generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
