#include "bisecting.hpp"

#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/distribution.hpp>
#include <model/k_means_init.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <processing/k_means/k_means.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/max.hpp>
#include <thrill/api/reduce_by_key.hpp>

size_t processing::Bisecting::get_initial_cluster_count(size_t requested) const {
  return std::min(requested, 2UL);
}

std::vector<model::Pixel> processing::Bisecting::perform(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) const {
  auto size = get_initial_cluster_count(model.requested);
  KMeans k_means(size, 1UL, 0UL, model::KMeansInit::RANDOM, model::Distribution::LLOYD);
  auto current_model = k_means.process(ctx, pixels);
  for (size_t i = size; i < model.requested; i++) {
    auto closest = pixels.Map([&](const model::Pixel& p) {
      size_t cluster_index = current_model.nearest_cluster(p, ctx.rank);
      return model::Cluster{p, cluster_index, 1};
    });
    auto accumulated = closest.ReduceByKey(
        [](const model::Cluster& c) {
          return c.cluster_index;
        },
        [](const model::Cluster& a, const model::Cluster& b) {
          return model::Cluster{a.center + b.center, a.cluster_index, a.count + b.count};
        }
    );
    // TODO: accumulated clusters were already computed in previous k-Means step
    // should the KMeansModel just contain the index of the biggest cluster? <-- Overhead?
    auto max_cluster = accumulated.Max().cluster_index;
    auto filtered = closest.Filter([max_cluster](const model::Cluster& cluster) {
      return cluster.cluster_index == max_cluster;
    });
    auto filtered_points = filtered.Map([](const model::Cluster& cluster) {
      return cluster.center;
    });
    // replace the biggest cluster by calculating 2 new ones using k-Means
    auto next_model = k_means.process(ctx, filtered_points.Collapse());
    current_model.centers.erase(current_model.centers.begin() + static_cast<long>(max_cluster));
    current_model.centers.insert(current_model.centers.end(), next_model.centers.begin(), next_model.centers.end());
  }
  return current_model.centers;
}
