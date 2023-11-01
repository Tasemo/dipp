#pragma once

#include <processing/k_means/init/init_strategy.hpp>

namespace processing {

/**
 * @brief implementation of a k-Means++ init strategy
 *
 * @see Arthur und Vassilvitskii, "k-means++: The Advantages of Careful Seeding", 2007
 */
class KMeansPlusPlus : public InitStrategy {
 public:
  std::vector<model::Pixel> generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
