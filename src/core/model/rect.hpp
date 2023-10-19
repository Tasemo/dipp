#pragma once

#include <cassert>
#include <cstddef>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <ostream>
#include <thrill/data/serialization_cereal.hpp>

namespace model {

/**
 * @brief simple rectangle implementation, defined by its 4 corners
 *
 * @note - needed, because cv::Rect is not serializable by thrill
 */
struct Rect {
  size_t min_x{}, max_x{}, min_y{}, max_y{};

  Rect() = default;

  Rect(size_t min_x, size_t max_x, size_t min_y, size_t max_y)
      : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y) {
    assert(min_x <= max_x);
    assert(min_y <= max_y);
  }

  bool contains(const model::Pixel& pixel) const {
    return contains(pixel.location);
  }

  bool contains(const cv::Vec2d& point) const {
    return contains(point[0], point[1]);
  }

  bool contains(size_t x, size_t y) const {
    return x >= min_x && x <= max_x && y >= min_y && y <= max_y;
  }

  bool is_on_edge(const model::Pixel& pixel) const {
    return is_on_edge(pixel.location);
  }

  bool is_on_edge(const cv::Vec2d& point) const {
    return is_on_edge(point[0], point[1]);
  }

  bool is_on_edge(size_t x, size_t y) const {
    return (x >= min_x && x <= max_x && (y == min_y || y == max_y)) ||
           (y >= min_y && y <= max_y && (x == min_x || x == max_x));
  }

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(min_x, max_x, min_y, max_y);
  }
};

inline std::ostream& operator<<(std::ostream& os, const Rect& rect) {
  return os << '(' << rect.min_x << ',' << rect.max_x << ',' << rect.min_y << ',' << rect.max_y << ')';
}

}  // namespace model
