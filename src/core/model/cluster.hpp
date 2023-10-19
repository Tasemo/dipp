#pragma once

#include <cstddef>
#include <model/pixel.hpp>
#include <ostream>
#include <thrill/data/serialization_cereal.hpp>

namespace model {

/**
 * @brief a data structure that keeps track of the center and amount of pixels in a kMeans cluster
 *
 * @note - associated pixels are not stored for performance reasons, recompute them after a run
 */
struct Cluster {
  model::Pixel center;
  size_t cluster_index{};
  size_t count{};

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(center, cluster_index, count);
  }
};

inline std::ostream& operator<<(std::ostream& os, const Cluster& cluster) {
  return os << '(' << cluster.center << ',' << cluster.cluster_index << ',' << cluster.count << ')';
}

}  // namespace model
