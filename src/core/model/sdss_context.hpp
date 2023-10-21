#pragma once

#include <model/context.hpp>
#include <string>
#include <util/string_utils.hpp>

namespace model {

static const double ARCSEC_TO_DEGREE = 1 / 3600.0;

/**
 * @brief holds context about the SDSS data set and methods to convert celestial coordinates
 */
struct SDSSContext {
  /**
   * @brief the general context with information about the distributed image
   */
  const model::Context& context;
  /**
   * @brief the scale of how many arcseconds each pixel covers
   */
  const double scale_arc;
  /**
   * @brief the scale of how many degrees each pixel covers
   */
  const double scale_deg;
  /**
   * @brief the right-ascension of the top left corner of the global image
   */
  const double start_ra;
  /**
   * @brief the declination of the top left corner of the global image
   */
  const double start_dec;
  /**
   * @brief the right-ascesion of the top right corner of the global image
   */
  const double max_ra;
  /**
   * @brief the declination of the top right corner of the global image
   */
  const double max_dec;
  /**
   * @brief the right-ascesion of the top left corner of the current subimage
   */
  const double local_start_ra;
  /**
   * @brief the declination of the top left corner of the current subimage
   */
  const double local_start_dec;
  /**
   * @brief a key to identify unique SDSS data sets, based on starting point, size, scale and number of workers
   */
  const std::string file_key;

  /**
   * @brief constructs a new SDSS context with a given general context, the top left starting point and a pixel scale
   */
  SDSSContext(const model::Context& context, double start_ra, double start_dec, double scale_arc)
      : context(context),
        scale_arc(scale_arc),
        scale_deg(scale_arc * ARCSEC_TO_DEGREE),
        start_ra(start_ra),
        start_dec(start_dec),
        max_ra(start_ra - context.global_width * scale_deg),
        max_dec(start_dec - context.global_height * scale_deg),
        local_start_ra(start_ra - context.x_tile * context.local_width * scale_deg),
        local_start_dec(start_dec - context.y_tile * context.local_height * scale_deg),
        file_key(
            util::to_string(context.ctx.num_workers()) + 'x' +
            util::to_string(start_ra) + 'x' +
            util::to_string(start_dec) + 'x' +
            util::to_string(context.local_width) + 'x' +
            util::to_string(context.local_height) + 'x' +
            util::to_string(scale_arc)
        ) {}

  /**
   * @brief converts the given right-ascension from the top left to the center of the image
   */
  inline double to_center_ra(double ra) const {
    return ra - context.local_width / 2.0 * scale_deg;
  }

  /**
   * @brief converts the given declination from the top left to the center of the image
   */
  inline double to_center_dec(double dec) const {
    return dec - context.local_height / 2.0 * scale_deg;
  }
};

}  // namespace model
