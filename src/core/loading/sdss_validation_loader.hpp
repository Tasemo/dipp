#pragma once

#include <lift/response.hpp>
#include <model/rect.hpp>
#include <model/sdss_context.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace loading {

class SDSSValidationLoader {
 private:
  const model::SDSSContext& _sdss;

  lift::response load_from_network() const;
  std::vector<model::Rect> convert_data(std::string_view data) const;

 public:
  explicit SDSSValidationLoader(const model::SDSSContext& sdss);
  std::vector<model::Rect> load_bounding_boxes(const std::string& data_path) const;
};

}  // namespace loading
