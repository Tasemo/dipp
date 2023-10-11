#pragma once

#include <cstddef>
#include <model/pixel.hpp>
#include <model/rect.hpp>
#include <optional>
#include <processing/pipeline.hpp>
#include <thrill/api/dia.hpp>
#include <unordered_set>
#include <utility>
#include <vector>

namespace processing {

/**
 * @brief result of the validation with the number of cut/uncut clusters
 */
struct Result {
  size_t total_clusters = 0;
  size_t cut_clusters = 0;
  size_t uncut_clusters = 0;
  size_t not_found_clusters = 0;
};

/**
 * @brief validates the found clusters by checking that no two clusters contain data from the same bounding box
 */
class Validation : public processing::Pipeline<thrill::DIA<std::vector<model::Pixel>>, Result> {
 private:
  using Pair = std::pair<size_t, size_t>;

  const std::vector<model::Rect>& _bounding_boxes;

  std::unordered_set<size_t> get_matching_clusters(const std::vector<model::Pixel>& cluster) const;
  std::optional<size_t> get_cluster_by_pixel(const model::Pixel& pixel) const;

 public:
  explicit Validation(const std::vector<model::Rect>& bounding_boxes);
  Result process(const model::Context& /*ctx*/, const thrill::DIA<std::vector<model::Pixel>>& clusters) const override;
};

}  // namespace processing
