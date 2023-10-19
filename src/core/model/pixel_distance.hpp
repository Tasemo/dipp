#pragma once

#include <model/pixel.hpp>
#include <utility>
#include <thrill/data/serialization_cereal.hpp>

namespace model {

struct PixelDistance {
  model::Pixel pixel{};
  double distance{};

  PixelDistance() = default;

  PixelDistance(model::Pixel pixel, double distance)
      : pixel(std::move(pixel)), distance(distance) {}

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(pixel, distance);
  }
};

inline bool operator<(const PixelDistance& lhs, const PixelDistance& rhs) {
  return lhs.distance < rhs.distance;
}

inline bool operator>(const PixelDistance& lhs, const PixelDistance& rhs) {
  return lhs.distance > rhs.distance;
}

inline bool operator<=(const PixelDistance& lhs, const PixelDistance& rhs) {
  return lhs.distance <= rhs.distance;
}

inline bool operator>=(const PixelDistance& lhs, const PixelDistance& rhs) {
  return lhs.distance >= rhs.distance;
}

}  // namespace model
