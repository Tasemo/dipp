#pragma once

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief groups all pixels of a cluster together so that they are on the same worker
 */
class PixelClusterMapping : public Pipeline<thrill::DIA<model::Pixel>, thrill::DIA<std::vector<model::Pixel>>> {
 private:
  const model::KMeansModel& _model;

 public:
  explicit PixelClusterMapping(const model::KMeansModel& model);
  thrill::DIA<std::vector<model::Pixel>> process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
