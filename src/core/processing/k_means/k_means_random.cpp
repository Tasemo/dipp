#include "k_means_random.hpp"

#include <cassert>
#include <cstddef>
#include <model/pixel.hpp>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/sample.hpp>
#include <vector>

std::vector<model::Pixel> processing::KMeansRandom::generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const {
  assert(cluster_count > 0);
  auto cached = pixels.Cache();
  return cached.Sample(cluster_count).AllGather();
}
