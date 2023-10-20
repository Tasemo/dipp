#include "sdss_image_loader.hpp"

#include <opencv2/core/hal/interface.h>

#include <filesystem>
#include <lift/http.hpp>
#include <lift/request.hpp>
#include <lift/response.hpp>
#include <model/sdss_context.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <util/file_utils.hpp>
#include <util/network_utils.hpp>
#include <util/query_builder.hpp>

static const std::string BASE_URL = "https://skyserver.sdss.org/dr18/SkyServerWS/ImgCutout/getjpeg?";
static const std::string DATA_LOCATION = "/data/sdss/";

lift::response loading::SDSSImageLoader::load_from_network(const std::string& options) const {
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

cv::Mat loading::SDSSImageLoader::load(const std::string& data_dir, const std::string& options) const {
  std::string data_path = data_dir + std::to_string(_sdss.context.rank) + ".jpeg";
  if (std::filesystem::exists(data_path)) {
    return cv::imread(data_path, cv::IMREAD_COLOR);
  }
  auto response = load_from_network(options);
  std::filesystem::create_directories(data_dir);
  util::write_to_file(data_path, response.data(), true);
  std::string buffer(response.data());
  cv::Mat raw_data(1, buffer.size(), CV_8UC1, static_cast<void*>(buffer.data()));
  return cv::imdecode(raw_data, cv::IMREAD_COLOR);
}

loading::SDSSImageLoader::SDSSImageLoader(const model::SDSSContext& sdss)
    : _sdss(sdss) {}

std::string loading::SDSSImageLoader::get_data_dir() const {
  return DATA_LOCATION + _sdss.file_key + '/';
}

cv::Mat loading::SDSSImageLoader::load_image() {
  return load(get_data_dir());
}

cv::Mat loading::SDSSImageLoader::load_validation() {
  return load(get_data_dir() + "validation/", "B");
}
