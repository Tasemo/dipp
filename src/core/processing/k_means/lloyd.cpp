#include "lloyd.hpp"

#include <cstddef>
#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>

std::vector<model::Pixel> processing::Lloyd::perform(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) const {
  auto closest = pixels.Map([&model, &ctx](const model::Pixel& p) {
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
  return new_local_centers.Collapse().AllGather();
}
