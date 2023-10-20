#include "pixel_cluster_mapping.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/group_by_key.hpp>
#include <vector>

processing::PixelClusterMapping::PixelClusterMapping(const model::KMeansModel& model)
    : _model(model) {}

thrill::DIA<std::vector<model::Pixel>> processing::PixelClusterMapping::process(const model::Context& /*ctx*/, const thrill::DIA<model::Pixel>& pixels) const {
  return pixels.GroupByKey<std::vector<model::Pixel>>(
      [&](const model::Pixel& p) {
        return _model.nearest_cluster(p);
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
