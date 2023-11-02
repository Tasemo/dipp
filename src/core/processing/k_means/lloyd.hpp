#pragma once

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/k_means/k_means_strategy.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief implementation of the lloyd k-Means algorithm
 */
class Lloyd : public KMeansStrategy {
 public:
  std::vector<model::Pixel> perform(const model::Context& ctx, model::KMeansModel model, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
