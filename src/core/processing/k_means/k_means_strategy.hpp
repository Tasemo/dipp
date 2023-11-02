#pragma once

#include <cstddef>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief interface for a k-means implementation
 */
class KMeansStrategy {
 public:
  virtual ~KMeansStrategy() = default;
  virtual std::vector<model::Pixel> perform(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) const = 0;

  virtual size_t get_initial_cluster_count(size_t requested) const {
    return requested;
  }
};

}  // namespace processing
