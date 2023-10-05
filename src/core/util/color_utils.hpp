#pragma once

#include <cstddef>
#include <opencv2/core/matx.hpp>
#include <vector>

namespace util {

/**
 * @brief generates n evenly spaced color values in the BGR format with a optional minimal distance between colors
 *
 * @note - this does not care about perceived human color difference, only numeric values are spaced evenly
 * @note - there are 1792 possible colors if you do no want space between colors
 * @note - if the amount of available colors is exceeded, color are repeated from the beginning
 *
 * @param amount the amount of colors to generate
 * @param minimal_space the amount of space between consecutive color values, default: 0
 */
std::vector<cv::Vec3b> generate_colors(size_t amount, size_t minimal_space = 0);

}  // namespace util
