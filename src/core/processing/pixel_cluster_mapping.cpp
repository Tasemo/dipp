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
  // GroupByKey calls the key_extractor function multiple times, computing the nearest cluster only once saves a ton of time
  using Pair = std::pair<model::Pixel, size_t>;
  auto pre_mapped = pixels.Map([&](const model::Pixel& p) {
    return Pair(p, _model.nearest_cluster(p));
  });
  return pre_mapped.GroupByKey<std::vector<model::Pixel>>(
      [&](const Pair& p) {
        return p.second;
      },
      [](auto& iterator, size_t) {
        std::vector<model::Pixel> data;
        while (iterator.HasNext()) {
          data.push_back(iterator.Next().first);
        }
        return data;
      }
  );
}
