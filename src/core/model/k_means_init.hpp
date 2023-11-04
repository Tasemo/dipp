#pragma once

#include <cstddef>
#include <ostream>

namespace model {

enum struct KMeansInit : size_t {
  RANDOM = 1,
  RANDOM_PARTITION = 2,
  K_MEANS_PP = 3
};

inline bool is_valid(KMeansInit /*distribution*/, size_t value) {
  return value >= static_cast<size_t>(KMeansInit::RANDOM) && value <= static_cast<size_t>(KMeansInit::K_MEANS_PP);
}

inline std::ostream& operator<<(std::ostream& os, const KMeansInit& k_means_init) {
  switch (k_means_init) {
    case KMeansInit::RANDOM:
      return os << "RANDOM";
    case KMeansInit::RANDOM_PARTITION:
      return os << "RANDOM_PARTITION";
    case KMeansInit::K_MEANS_PP:
      return os << "K_MEANS_PP";
  }
}

}  // namespace model
