#include "processing/k_means/init/random_partition.hpp"

#include <cassert>
#include <cstddef>
#include <model/cluster.hpp>
#include <model/pixel.hpp>
#include <random>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <vector>

std::vector<model::Pixel> processing::KMeansRandomPartition::generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const {
  assert(cluster_count > 0);
  auto cached = pixels.Cache();
  std::random_device random;
  std::mt19937 generator(random());
  std::uniform_int_distribution<std::mt19937::result_type> distribution(1, cluster_count);
  auto random_clusters = pixels.Map([&](const model::Pixel& p) {
    size_t random_cluster = distribution(generator);
    return model::Cluster{p, random_cluster, 1};
  });
  auto clusters = random_clusters.ReduceByKey(
      [](const model::Cluster& c) {
        return c.cluster_index;
      },
      [](const model::Cluster& a, const model::Cluster& b) {
        return model::Cluster{a.center + b.center, a.cluster_index, a.count + b.count};
      }
  );
  auto mean_clusters = clusters.Map([](const model::Cluster& c) {
    return c.center / c.count;
  });
  return mean_clusters.AllGather();
}
