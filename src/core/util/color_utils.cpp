#include "color_utils.hpp"

#include <cassert>

static const double FRACTION_SIZE = 1.0 / 7.0;

cv::Vec3b get(double fraction) {
  assert(fraction >= 0 && fraction <= 1.0);
  if (fraction >= 0 * FRACTION_SIZE && fraction < 1 * FRACTION_SIZE) {
    unsigned char value = (fraction - 0 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{value, 0, 0};
  } else if (fraction >= 1 * FRACTION_SIZE && fraction < 2 * FRACTION_SIZE) {
    unsigned char value = (fraction - 1 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{0, value, 0};
  } else if (fraction >= 2 * FRACTION_SIZE && fraction < 3 * FRACTION_SIZE) {
    unsigned char value = (fraction - 2 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{0, 0, value};
  } else if (fraction >= 3 * FRACTION_SIZE && fraction < 4 * FRACTION_SIZE) {
    unsigned char value = (fraction - 3 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{value, value, 0};
  } else if (fraction >= 4 * FRACTION_SIZE && fraction < 5 * FRACTION_SIZE) {
    unsigned char value = (fraction - 4 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{value, 0, value};
  } else if (fraction >= 5 * FRACTION_SIZE && fraction < 6 * FRACTION_SIZE) {
    unsigned char value = (fraction - 5 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{0, value, value};
  } else {
    unsigned char value = (fraction - 6 * FRACTION_SIZE) / FRACTION_SIZE * 255;
    return cv::Vec3b{value, value, value};
  }
}

std::vector<cv::Vec3b> util::generate_colors(size_t amount, size_t minimal_space) {
  std::vector<cv::Vec3b> result;
  result.reserve(amount);
  size_t maximum = 7L * 256 / (minimal_space + 1);
  size_t actual = std::min(maximum, amount);
  double dx = 1.0 / (actual - 1);
  for (size_t i = 0; i < amount; i++) {
    result.push_back(get(i % actual * dx));
  }
  return result;
}
