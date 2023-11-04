#pragma once

#include <cstddef>
#include <ostream>

namespace model {

enum struct Distribution : size_t {
  TRIVIAL = 1,
  LLOYD = 2,
  BISECTING = 3,
};

inline bool is_valid(Distribution /*distribution*/, size_t value) {
  return value >= static_cast<size_t>(Distribution::TRIVIAL) && value <= static_cast<size_t>(Distribution::BISECTING);
}

inline std::ostream& operator<<(std::ostream& os, const Distribution& distribution) {
  switch (distribution) {
    case Distribution::TRIVIAL:
      return os << "TRIVIAl";
    case Distribution::LLOYD:
      return os << "LLOYD";
    case Distribution::BISECTING:
      return os << "BISECTING";
    default:
      throw std::invalid_argument("Unknown enum!");
  }
}

}  // namespace model
