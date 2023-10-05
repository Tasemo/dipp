#include "k_means.hpp"

#include <cstddef>
#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/sample.hpp>
#include <vector>

processing::KMeans::KMeans(size_t cluster_count, size_t max_iteratations, size_t epsilon)
    : _cluster_count(cluster_count), _max_iteratations(max_iteratations), _epsilon(epsilon) {}

bool processing::KMeans::epsilon_reached(const model::KMeansModel& model, const std::vector<model::Pixel>& new_centers) const {
  for (size_t i = 0; i < new_centers.size(); i++) {
    if (new_centers[i].distance_to(model.centers[i]) > _epsilon) {
      return false;
    }
  }
  return true;
}

model::KMeansModel processing::KMeans::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  model::KMeansModel model;
  auto cached_pixels = pixels.Cache();
  model.centers = cached_pixels.Sample(_cluster_count).AllGather();
  bool break_condition = false;
  for (size_t i = 0; i < _max_iteratations && !break_condition; i++) {
    auto closest = cached_pixels.Map([&model, &ctx](const model::Pixel& p) {
      size_t cluster_index = model.nearest_cluster(p, ctx.rank);
      return model::Cluster{p, cluster_index, 1};
    });
    auto clusters = closest.ReduceByKey(
        [](const model::Cluster& c) {
          return c.cluster_index;
        },
        [](const model::Cluster& a, const model::Cluster& b) {
          return model::Cluster{a.center + b.center, a.cluster_index, a.count + b.count};
        }
    );
    auto new_local_centers = clusters.Map([](const model::Cluster& c) {
      return c.center / c.count;
    });
    std::vector<model::Pixel> new_centers = new_local_centers.Collapse().AllGather();
    if (_epsilon > 0) {
      break_condition = epsilon_reached(model, new_centers);
    }
    model.iterations++;
    model.centers = new_centers;
  }
  return model;
}
