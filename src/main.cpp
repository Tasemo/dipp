#include <loading/sdss_image_loader.hpp>
#include <thrill/api/context.hpp>

void process(thrill::Context &ctx) {}

int main() {
  loading::SDSSImageLoader loader;
  auto image_path = loader.load_image();
  return thrill::api::Run([&](thrill::api::Context &ctx) {
    process(ctx);
  });
}
