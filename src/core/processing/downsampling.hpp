#pragma once

#include <model/context.hpp>
#include <model/pixel.hpp>
#include <processing/pipeline.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>

namespace processing {

/**
 * @brief a processor that reduces the data size by a given factor
 *
 * @note data is reduced evenly in a 2D space in a chessboard-like pattern
 */
class Downsampling : public Pipeline<thrill::DIA<model::Pixel>, thrill::DIA<model::Pixel>> {
 private:
  const size_t _factor;

 public:
  explicit Downsampling(size_t factor);
  thrill::DIA<model::Pixel> process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
