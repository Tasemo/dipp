#include <loading/sdss_image_loader.hpp>
#include <model/context.hpp>
#include <model/sdss_context.hpp>
#include <thrill/api/context.hpp>

struct Args {
  size_t global_width{1024};
  size_t global_height{1024};
  double start_ra{180.0};
  double start_dec{0.0};
  double scale{1.0};
};

void process(thrill::Context &ctx, const Args &args) {
  model::Context context(ctx, args.global_width, args.global_height);
  model::SDSSContext sdss(context, args.start_ra, args.start_dec, args.scale);
  loading::SDSSImageLoader loader(sdss);
  auto image_path = loader.load_image();
}

int main() {
  Args args{};
  return thrill::api::Run([&args](thrill::api::Context &ctx) {
    process(ctx, args);
  });
}
