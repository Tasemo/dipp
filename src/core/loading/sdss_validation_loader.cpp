#include "sdss_validation_loader.hpp"

#include <filesystem>
#include <lift/request.hpp>
#include <string_view>
#include <util/csv_reader.hpp>
#include <util/file_utils.hpp>
#include <util/math_utils.hpp>
#include <util/network_utils.hpp>
#include <util/query_builder.hpp>

static const std::string BASE_URL = "https://skyserver.sdss.org/dr18/SkyServerWS/SearchTools/SqlSearch?";
static const size_t FIELD_X_PIXELS = 2048;
static const size_t FIELD_Y_PIXELS = 1489;
static const std::string PRE_SQL =
    "SELECT f.raMin, f.raMax, f.decMin, f.decMax, ao.rmin, ao.rmax, ao.cmin, ao.cmax "
    "FROM PhotoPrimary AS p "
    "JOIN fGetObjFromRect(";
static const std::string POST_SQL =
    ") AS r "
    "ON p.objID = r.objID "
    "JOIN AtlasOutline AS ao "
    "ON p.objID = ao.objID "
    "JOIN Field as f "
    "ON p.fieldID = f.fieldID ";
// "WHERE p.clean = 1";

lift::response loading::SDSSValidationLoader::load_from_network() const {
  std::string sql_command =
      PRE_SQL +
      std::to_string(_sdss.max_ra) + "," +
      std::to_string(_sdss.start_ra) + "," +
      std::to_string(_sdss.max_dec) + ", " +
      std::to_string(_sdss.start_dec) +
      POST_SQL;
  util::QueryBuilder query;
  query.add_part("format", "CSV")
      .add_part("cmd", sql_command);
  lift::request request{BASE_URL + query.build()};
  request.method(lift::http::method::get);
  return util::send_request(request, "SDSS BoundingBox");
}

std::vector<model::Rect> loading::SDSSValidationLoader::convert_data(std::string_view data) const {
  util::CSVReader csv(data, true);
  std::vector<model::Rect> result;
  double ra_min{}, ra_max{}, dec_min{}, dec_max{};
  size_t row_min{}, row_max{}, col_min{}, col_max{};
  while (csv.read_line(ra_min, ra_max, dec_min, dec_max, row_min, row_max, col_min, col_max)) {
    // convert outlines from field coordinates to celestial coordinates
    double celestial_min_x = util::map_range(ra_min, ra_max, 0UL, FIELD_Y_PIXELS, row_min);
    double celestial_max_x = util::map_range(ra_min, ra_max, 0UL, FIELD_Y_PIXELS, row_max);
    double celestial_min_y = util::map_range(dec_min, dec_max, 0UL, FIELD_X_PIXELS, col_min);
    double celestial_max_y = util::map_range(dec_min, dec_max, 0UL, FIELD_X_PIXELS, col_max);
    // filter objects (partially) outside of the global image
    if (celestial_min_x < _sdss.max_ra || celestial_max_x > _sdss.start_ra ||
        celestial_min_y < _sdss.max_dec || celestial_max_y > _sdss.start_dec) {
      continue;
    }
    // convert celestial coordinates to global image coordinates
    size_t min_x = util::map_range(0UL, _sdss.context.global_width, _sdss.start_ra, _sdss.max_ra, celestial_min_x);
    size_t max_x = util::map_range(0UL, _sdss.context.global_width, _sdss.start_ra, _sdss.max_ra, celestial_max_x);
    size_t min_y = util::map_range(0UL, _sdss.context.global_height, _sdss.start_dec, _sdss.max_dec, celestial_min_y);
    size_t max_y = util::map_range(0UL, _sdss.context.global_height, _sdss.start_dec, _sdss.max_dec, celestial_max_y);
    result.emplace_back(max_x, min_x, max_y, min_y);
  }
  return result;
}

loading::SDSSValidationLoader::SDSSValidationLoader(const model::SDSSContext& sdss)
    : _sdss(sdss) {}

std::vector<model::Rect> loading::SDSSValidationLoader::load_bounding_boxes(const std::string& data_path) const {
  if (!std::filesystem::exists(data_path)) {
    auto response = load_from_network();
    auto data = response.data();
    util::write_to_file(data_path, data);
    return convert_data(data);
  } else {
    auto data = util::read_from_file(data_path);
    return convert_data(data);
  }
}
