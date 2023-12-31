#include "pixel_worker_mapping.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/group_by_key.hpp>
#include <vector>

thrill::DIA<std::vector<model::Pixel>> processing::PixelWorkerMapping::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  return pixels.GroupByKey<std::vector<model::Pixel>>(
      [&](const model::Pixel&) {
        return ctx.rank;
      },
      [](auto& iterator, size_t) {
        std::vector<model::Pixel> data;
        while (iterator.HasNext()) {
          data.push_back(iterator.Next());
        }
        return data;
      }
  );
}
