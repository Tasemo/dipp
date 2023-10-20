#pragma once

#include <cstddef>
#include <model/pixel.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief interface for a k-means initialization strategy
 */
class KMeansInit {
 public:
  virtual ~KMeansInit() = default;
  virtual std::vector<model::Pixel> generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const = 0;
};

}  // namespace processing
