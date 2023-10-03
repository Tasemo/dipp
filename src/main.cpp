#include <loading/sdss_image_loader.hpp>
#include <thrill/api/context.hpp>

void process(thrill::Context &ctx) {
  loading::SDSSImageLoader loader;
  auto image_path = loader.load_image();
}

int main() {
  return thrill::api::Run([&](thrill::api::Context &ctx) {
    process(ctx);
  });
}
