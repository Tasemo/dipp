#pragma once

#include <cstddef>

namespace model {

enum Distribution {
  TRIVIAL = 1,
  LLOYD = 2,
  BISECTING = 3,
};

inline bool is_valid_distribution(size_t value) {
  return value >= static_cast<size_t>(Distribution::TRIVIAL) && value <= static_cast<size_t>(Distribution::BISECTING);
}

inline Distribution get_distribution(size_t value) {
  return static_cast<Distribution>(value);
}

}  // namespace model
