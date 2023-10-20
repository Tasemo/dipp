#include "validation.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <model/rect.hpp>
#include <optional>
#include <thrill/api/all_gather.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <unordered_set>
#include <vector>

processing::Validation::Validation(const std::vector<model::Rect>& bounding_boxes)
    : _bounding_boxes(bounding_boxes) {}

std::optional<size_t> processing::Validation::get_cluster_by_pixel(const model::Pixel& pixel) const {
  for (size_t i = 0; i < _bounding_boxes.size(); i++) {
    model::Rect box = _bounding_boxes[i];
    if (box.contains(pixel)) {
      return i;
    }
  }
  return std::nullopt;
}

std::unordered_set<size_t> processing::Validation::get_matching_clusters(const std::vector<model::Pixel>& cluster) const {
  std::unordered_set<size_t> matching_clusters;
  for (const model::Pixel& pixel : cluster) {
    std::optional<size_t> found_cluster = get_cluster_by_pixel(pixel);
    if (found_cluster.has_value()) {
      matching_clusters.insert(*found_cluster);
    }
  }
  return matching_clusters;
}

processing::Result processing::Validation::process(const model::Context& /*ctx*/, const thrill::DIA<std::vector<model::Pixel>>& clusters) const {
  auto local_found = clusters.FlatMap<Pair>([&](const std::vector<model::Pixel>& cluster, auto emit) {
    auto matching_clusters = get_matching_clusters(cluster);
    for (const auto& matching : matching_clusters) {
      emit(Pair(matching, 1));
    }
  });
  auto cut_clusters = local_found.ReduceByKey(
      [](const Pair& matching) {
        return matching.first;
      },
      [](const Pair& a, const Pair& b) {
        return Pair(a.first, a.second + b.second);
      }
  );
  std::vector<Pair> all_cuts = cut_clusters.AllGather();
  Result result;
  for (const Pair& possible_cut : all_cuts) {
    if (possible_cut.second == 1) {
      result.uncut_clusters++;
    } else {
      result.cut_clusters++;
    }
  }
  result.total_clusters = _bounding_boxes.size();
  result.not_found_clusters = result.total_clusters - result.cut_clusters - result.uncut_clusters;
  return result;
}
