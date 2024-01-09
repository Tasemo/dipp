#include "processing/k_means/k_means.hpp"

#include <cstddef>
#include <memory>
#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/distribution.hpp>
#include <model/k_means_init.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/k_means/bisecting.hpp>
#include <processing/k_means/init/init_strategy.hpp>
#include <processing/k_means/init/k_means_plus_plus.hpp>
#include <processing/k_means/init/random.hpp>
#include <processing/k_means/init/random_partition.hpp>
#include <processing/k_means/k_means_strategy.hpp>
#include <processing/k_means/lloyd.hpp>
#include <stdexcept>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/sample.hpp>
#include <util/string_utils.hpp>
#include <vector>

std::unique_ptr<processing::InitStrategy> get_init_strategy(model::KMeansInit init) {
  switch (init) {
    case model::KMeansInit::RANDOM:
      return std::make_unique<processing::KMeansRandom>();
    case model::KMeansInit::RANDOM_PARTITION:
      return std::make_unique<processing::KMeansRandomPartition>();
    case model::KMeansInit::K_MEANS_PP:
      return std::make_unique<processing::KMeansPlusPlus>();
    default:
      throw std::invalid_argument("Unknown init option: " + util::to_string(init));
  }
}

std::unique_ptr<processing::KMeansStrategy> get_implementation(model::Distribution distribution) {
  switch (distribution) {
    case model::Distribution::LLOYD:
      return std::make_unique<processing::Lloyd>();
    case model::Distribution::BISECTING:
      return std::make_unique<processing::Bisecting>();
    default:
      throw std::invalid_argument("Unknown distribution option: " + util::to_string(distribution));
      break;
  }
}

processing::KMeans::KMeans(size_t cluster_count, size_t max_iteratations, size_t epsilon, model::KMeansInit init, model::Distribution distribution)
    : _cluster_count(cluster_count),
      _max_iteratations(max_iteratations),
      _epsilon(epsilon),
      _init(get_init_strategy(init)),
      _impl(get_implementation(distribution)) {}

bool processing::KMeans::epsilon_reached(const model::KMeansModel& model, const std::vector<model::Pixel>& new_centers) const {
  for (size_t i = 0; i < new_centers.size(); i++) {
    if (new_centers[i].distance_to(model.centers[i]) > _epsilon) {
      return false;
    }
  }
  return true;
}

model::KMeansModel processing::KMeans::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels, size_t cluster_count) const {
  model::KMeansModel model;
  model.requested = cluster_count;
  auto cached_pixels = pixels.Cache();
  size_t initial_count = _impl->get_initial_cluster_count(cluster_count);
  if (initial_count == cluster_count) {
    model.centers = _init->generate(initial_count, cached_pixels);
    bool break_condition = false;
    for (size_t i = 0; i < _max_iteratations && !break_condition; i++) {
      auto new_centers = _impl->perform(ctx, model, cached_pixels);
      if (_epsilon > 0) {
        break_condition = epsilon_reached(model, new_centers);
      }
      model.iterations++;
      model.centers = new_centers;
    }
  } else {
    model.centers = _impl->perform(ctx, model, cached_pixels);
  }
  return model;
}

model::KMeansModel processing::KMeans::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  return process(ctx, pixels, _cluster_count);
}

model::KMeansModel processing::KMeans::process(const model::Context& ctx, const ClusterEstimation& estimation) const {
  if (ctx.rank == 0) {
    std::cout << "Estimated: " << estimation.global_cluster_count << "\n";
  }
  return process(ctx, estimation.pixels, estimation.global_cluster_count);
}
