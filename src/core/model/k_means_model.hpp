#pragma once

#include <cstddef>
#include <model/pixel.hpp>
#include <vector>

namespace model {

struct KMeansModel {
  size_t iterations{};
  std::vector<model::Pixel> centers;

  size_t nearest_cluster(const model::Pixel& pixel, size_t start = 0) const {
    double min_dist = pixel.distance_to(centers[start]);
    size_t min_index = start;
    for (size_t i = 0; i < centers.size(); i++) {
      if (i != start) {
        double dist = pixel.distance_to(centers[i]);
        if (dist < min_dist) {
          min_dist = dist;
          min_index = i;
        }
      }
    }
    return min_index;
  }

  model::Pixel nearest_center(const model::Pixel& pixel) const {
    return centers[nearest_cluster(pixel)];
  }
};

}  // namespace model
