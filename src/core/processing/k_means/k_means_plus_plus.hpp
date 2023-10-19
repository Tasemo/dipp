#pragma once

#include <processing/k_means/k_means_init.hpp>

namespace processing {

class KMeansPlusPlus : public KMeansInit {
 public:
  std::vector<model::Pixel> generate(size_t cluster_count, const thrill::DIA<model::Pixel>& pixels) const override;
};

}  // namespace processing
