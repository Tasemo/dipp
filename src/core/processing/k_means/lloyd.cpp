#define LOCAL_REDUCE

#include "lloyd.hpp"

#include <cstddef>
#include <model/cluster.hpp>
#include <model/context.hpp>
#include <model/k_means_model.hpp>
#include <model/pixel.hpp>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>

#ifdef LOCAL_REDUCE
#include <thrill/api/cache.hpp>
#include <unordered_map>
#endif

thrill::DIA<model::Cluster> get_mapped_clusters(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) {
  auto closest = pixels.Map([&model, &ctx](const model::Pixel& p) {
    size_t cluster_index = model.nearest_cluster(p, ctx.rank);
    return model::Cluster{p, cluster_index, 1};
  });
#ifndef LOCAL_REDUCE
  return closest;
#else
  std::unordered_map<size_t, model::Cluster> reduce_map;
  auto mapped = closest.Map([&reduce_map](const model::Cluster& c) {
    auto existing = reduce_map[c.cluster_index];
    reduce_map[c.cluster_index] = model::Cluster{c.center + existing.center, c.cluster_index, c.count + existing.count};
    return c;
  });
  auto deduped = mapped.FlatMap([&reduce_map](const model::Cluster& c, auto emit) {
    auto it = reduce_map.find(c.cluster_index);
    if (it != reduce_map.end()) {
      emit(it->second);
      reduce_map.erase(it);
    }
  });
  return deduped.Cache().Execute();
#endif
}

std::vector<model::Pixel> processing::Lloyd::perform(const model::Context& ctx, const model::KMeansModel& model, const thrill::DIA<model::Pixel>& pixels) const {
  auto closest = get_mapped_clusters(ctx, model, pixels);
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
