#include "k_means_plus_plus.hpp"

#include <cassert>
#include <cstddef>
#include <limits>
#include <model/pixel.hpp>
#include <model/pixel_distance.hpp>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/max.hpp>
#include <thrill/api/sample.hpp>
#include <vector>

std::vector<model::Pixel> processing::KMeansPlusPlus::generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const {
  assert(cluster_count > 0);
  auto cached = pixels.Cache();
  auto centers = cached.Sample(1).AllGather();
  for (size_t i = 0; i < cluster_count - 1; i++) {
    // each node calculates the pixel that is furthest from all previous centers
    auto min_distances = cached.Map([&centers](const model::Pixel& p) {
      double min_distance = std::numeric_limits<double>::max();
      for (const auto& center : centers) {
        double d = p.distance_to(center);
        if (d < min_distance) {
          min_distance = d;
        }
      }
      return model::PixelDistance(p, min_distance);
    });
    // find the node that is globally the farthest and add it to the centers
    auto global_max = min_distances.Max();
    centers.push_back(global_max.pixel);
  }
  return centers;
}
