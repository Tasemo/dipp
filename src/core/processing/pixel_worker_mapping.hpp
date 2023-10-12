#pragma once

#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>
#include <vector>

namespace processing {

/**
 * @brief groups all pixels of worker together
 */
class PixelWorkerMapping : public Pipeline<thrill::DIA<model::Pixel>, thrill::DIA<std::vector<model::Pixel>>> {
 public:
  thrill::DIA<std::vector<model::Pixel>> process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
