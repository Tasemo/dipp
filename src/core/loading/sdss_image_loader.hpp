#pragma once

#include <lift/response.hpp>
#include <model/sdss_context.hpp>
#include <string>

namespace loading {

class SDSSImageLoader {
 private:
  const model::SDSSContext& _sdss;

  lift::response load_from_network(const std::string& options) const;

 public:
  explicit SDSSImageLoader(const model::SDSSContext& sdss)
      : _sdss(sdss) {}

  std::string load_image();
};

}  // namespace loading
