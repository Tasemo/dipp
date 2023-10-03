#pragma once

#include <thrill/api/context.hpp>

namespace model {

struct Context {
  thrill::Context& ctx;
  const size_t rank;
  const size_t global_width, global_height;
  const size_t x_tiles, y_tiles;
  const size_t x_tile, y_tile;
  const size_t local_width, local_height;

  Context(thrill::Context& ctx, size_t global_width, size_t global_height)
      : ctx(ctx),
        rank(ctx.my_rank()),
        global_width(global_width),
        global_height(global_height),
        x_tiles(std::sqrt(ctx.num_workers())),
        y_tiles(std::sqrt(ctx.num_workers())),
        x_tile(rank % x_tiles),
        y_tile(rank / x_tiles),
        local_width(global_width / x_tiles),
        local_height(global_height / y_tiles) {}

  inline size_t to_local_index(size_t index) const {
    return index % (local_width * local_height);
  }

  inline size_t index_to_local_x(size_t index) const {
    return to_local_index(index) % local_width;
  }

  inline size_t index_to_local_y(size_t index) const {
    return to_local_index(index) / local_width;
  }

  inline size_t index_to_global_x(size_t index) const {
    return index_to_local_x(index) + x_tile * local_width;
  }

  inline size_t index_to_global_y(size_t index) const {
    return index_to_local_y(index) + y_tile * local_height;
  }

  inline size_t to_local_x(size_t global_x) const {
    return global_x % local_width;
  }

  inline size_t to_local_y(size_t global_y) const {
    return global_y % local_height;
  }

  inline size_t to_global_x(size_t local_x) const {
    return local_x + x_tile * local_width;
  }

  inline size_t to_global_y(size_t local_y) const {
    return local_y + y_tile * local_height;
  }
};

}  // namespace model
