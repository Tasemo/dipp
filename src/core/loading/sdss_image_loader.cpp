#include "sdss_image_loader.hpp"

#include <lift/request.hpp>
#include <string>
#include <util/network_utils.hpp>
#include <util/query_builder.hpp>

static const std::string BASE_URL = "https://skyserver.sdss.org/dr18/SkyServerWS/ImgCutout/getjpeg?";
static const std::string DATA_LOCATION = "/data/sdss/";

lift::response loading::SDSSImageLoader::load_from_network(const std::string& options = "") const {
  util::QueryBuilder query;
  query.add_part("width", _sdss.context.local_width)
      .add_part("height", _sdss.context.local_height)
      .add_part("ra", _sdss.to_center_ra(_sdss.local_start_ra))
      .add_part("dec", _sdss.to_center_dec(_sdss.local_start_dec))
      .add_part("scale", _sdss.scale_arc)
      .add_part("opt", options);
  lift::request request{BASE_URL + query.build()};
  request.method(lift::http::method::get);
  return util::send_request(request, "SDSS");
}

std::string loading::SDSSImageLoader::load_image() {
  return "/data";
}
