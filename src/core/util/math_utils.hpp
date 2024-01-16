#pragma once

namespace util {

/**
 * @brief performs a linear interpolation of a value from one range to another
 */
template <typename T, typename R>
inline R map_range(const R minOutRange, const R maxOutRange, const T minInRange, const T maxInRange, const T value) {
  auto slope = (maxOutRange - minOutRange) / (maxInRange - minInRange);
  return minOutRange + slope * (value - minInRange);
}

template <typename T>
inline double percentage(const T total, const T part) {
  return part / static_cast<double>(total) * 100;
}

}  // namespace util
