#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/size.hpp>

namespace processing {

/**
 * @brief a processor that applies a threshold filter (removes pixels based on color intesity)
 */
class Threshold : public Pipeline<thrill::DIA<model::Pixel>, thrill::DIA<model::Pixel>> {
 private:
  const unsigned char _cutoff;
  const bool _perceived;

 public:
  /**
   * @brief constructs a new threshold stage that filters pixels based on color intensity
   *
   * @param cutoff pixel itensities greater than this value are kept
   * @param perceived whether to use perceived color itensity
   */
  explicit Threshold(unsigned char cutoff, bool perceived = false);
  thrill::DIA<model::Pixel> process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
