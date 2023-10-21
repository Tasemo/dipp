#include "lloyd.hpp"

#include <cstddef>
#include <memory>
#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/k_means_init.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/k_means/k_means_init.hpp>
#include <processing/k_means/k_means_plus_plus.hpp>
#include <processing/k_means/k_means_random.hpp>
#include <stdexcept>
#include <string>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/sample.hpp>
#include <vector>

std::unique_ptr<processing::KMeansInit> get_implementation(model::KMeansInit init) {
  switch (init) {
    case model::KMeansInit::RANDOM:
      return std::make_unique<processing::KMeansRandom>();
    case model::KMeansInit::K_MEANS_PP:
      return std::make_unique<processing::KMeansPlusPlus>();
  }
  throw std::invalid_argument("Unknown init option: " + std::to_string(init));
}

processing::Lloyd::Lloyd(size_t cluster_count, size_t max_iteratations, size_t epsilon, model::KMeansInit init)
    : _cluster_count(cluster_count),
      _max_iteratations(max_iteratations),
      _epsilon(epsilon),
      _init(get_implementation(init)) {}

bool processing::Lloyd::epsilon_reached(const model::KMeansModel& model, const std::vector<model::Pixel>& new_centers) const {
  for (size_t i = 0; i < new_centers.size(); i++) {
    if (new_centers[i].distance_to(model.centers[i]) > _epsilon) {
      return false;
    }
  }
  return true;
}

model::KMeansModel processing::Lloyd::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels, size_t cluster_count) const {
  model::KMeansModel model;
  auto cached_pixels = pixels.Cache();
  model.centers = _init->generate(cluster_count, cached_pixels);
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

model::KMeansModel processing::Lloyd::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  return process(ctx, pixels, _cluster_count);
}

model::KMeansModel processing::Lloyd::process(const model::Context& ctx, const ClusterEstimation& estimation) const {
  std::cout << "Estimated: " << estimation.global_cluster_count << "\n";
  return process(ctx, estimation.pixels, estimation.global_cluster_count);
}
