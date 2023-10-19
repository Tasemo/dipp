#pragma once

#include <cstddef>

namespace model {

enum KMeansInit {
  RANDOM = 1,
  K_MEANS_PP = 2
};

inline bool is_valid_k_means_init(size_t value) {
  return value >= static_cast<size_t>(KMeansInit::RANDOM) && value <= static_cast<size_t>(KMeansInit::K_MEANS_PP);
}

inline KMeansInit get_k_means_init(size_t value) {
  return static_cast<KMeansInit>(value);
}

}  // namespace model
