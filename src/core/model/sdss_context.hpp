#pragma once

#include <model/context.hpp>
#include <string>
#include <util/string_utils.hpp>

namespace model {

static const double ARCSEC_TO_DEGREE = 1 / 3600.0;

struct SDSSContext {
  const model::Context& context;
  const double scale_arc;
  const double scale_deg;
  const double start_ra, start_dec;
  const double max_ra, max_dec;
  const double local_max_ra, local_max_dec;
  const std::string file_key;

  SDSSContext(const model::Context& context, double start_ra, double start_dec, double scale_arc)
      : context(context),
        scale_arc(scale_arc),
        scale_deg(scale_arc * ARCSEC_TO_DEGREE),
        start_ra(start_ra),
        start_dec(start_dec),
        max_ra(start_ra - context.global_width * scale_deg),
        max_dec(start_dec - context.global_height * scale_deg),
        local_max_ra(start_ra - context.x_tile * context.local_width * scale_deg),
        local_max_dec(start_dec - context.y_tile * context.local_height * scale_deg),
        file_key(
            util::to_string(context.ctx.num_workers()) + 'x' +
            util::to_string(start_ra) + 'x' +
            util::to_string(start_dec) + 'x' +
            util::to_string(context.local_width) + 'x' +
            util::to_string(context.local_height)
        ) {}

  inline double to_center_ra(double ra) const {
    return ra - context.local_width / 2.0 * scale_deg;
  }

  inline double to_center_dec(double dec) const {
    return dec - context.local_height / 2.0 * scale_deg;
  }
};

}  // namespace model
