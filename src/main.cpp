#include <thrill/api/context.hpp>

void process(thrill::Context &ctx) {}

int main() {
  return thrill::api::Run([&](thrill::api::Context &ctx) {
    process(ctx);
  });
}
