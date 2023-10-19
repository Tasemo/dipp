#pragma once

#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <ostream>
#include <thrill/data/serialization_cereal.hpp>

namespace model {

/**
 * @brief data structure for a single pixel, holding its location and color value
 */
struct Pixel {
  cv::Vec2d location{};
  cv::Vec3b bgr_value{};

  /**
   * @brief constructs a new pixel with all values being 0
   */
  Pixel() = default;

  /**
   * @brief construct a new pixel from a location and color value
   */
  Pixel(const cv::Vec2d& location, const cv::Vec3b& bgr_value)
      : location(location), bgr_value(bgr_value) {}

  /**
   * @brief construct a new pixel from a location and color value
   */
  Pixel(size_t x, size_t y, const cv::Vec3b& bgr_value)
      : location(cv::Vec2d{static_cast<double>(x), static_cast<double>(y)}), bgr_value(bgr_value) {}

  /**
   * @brief calculates the distance between this and the given pixel
   */
  double distance_to(const Pixel& other) const {
    return cv::norm(location - other.location);
  }

  /**
   * @brief serializes the pixel, is normally called by thrill
   */
  template <typename Archive>
  void serialize(Archive& archive) {
    archive(location.val, bgr_value.val);
  }

  /**
   * @brief adds a pixels location to this one, returning a new pixel
   */
  Pixel operator+(const Pixel& b) const {
    return Pixel{location + b.location, bgr_value};
  }

  /**
   * @brief adds a pixels location to this one
   */
  Pixel& operator+=(const Pixel& b) {
    location += b.location;
    return *this;
  }

  /**
   * @brief subtracts a pixels location from this one, returning a new pixel
   */
  Pixel operator-(const Pixel& b) const {
    return Pixel{location - b.location, bgr_value};
  }

  /**
   * @brief subtracts a pixels location from this one
   */
  Pixel& operator-=(const Pixel& b) {
    location -= b.location;
    return *this;
  }

  /**
   * @brief multiplies this pixels location by a given scalar, returning a new pixel
   */
  Pixel operator*(double s) const {
    return Pixel{location * s, bgr_value};
  }

  /**
   * @brief multiplies this pixels location by a given scalar
   */
  Pixel& operator*=(double s) {
    location *= s;
    return *this;
  }

  /**
   * @brief divides this pixels location by a given scalar, returning a new pixel
   */
  Pixel operator/(double s) const {
    return Pixel{location / s, bgr_value};
  }

  /**
   * @brief divides this pixels location by a given scalar, returning a new pixel
   */
  Pixel& operator/=(double s) {
    location /= s;
    return *this;
  }
};

inline bool operator==(const Pixel& lhs, const Pixel& rhs) {
  return lhs.location == rhs.location && lhs.bgr_value == rhs.bgr_value;
}

inline bool operator!=(const Pixel& lhs, const Pixel& rhs) {
  return lhs.location != rhs.location || lhs.bgr_value != rhs.bgr_value;
}

inline std::ostream& operator<<(std::ostream& os, const Pixel& pixel) {
  return os << '(' << pixel.location << ',' << pixel.bgr_value << ')';
}

}  // namespace model
