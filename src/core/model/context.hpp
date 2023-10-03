#pragma once

#include <thrill/api/context.hpp>

namespace model {

/**
 * @brief holds context about the distributed image and methods to convert between local and global coordinates and indices
 */
struct Context {
  /**
   * @brief the thrill context with information about the distribution
   */
  thrill::Context& ctx;
  /**
   * @brief the unique id of the current worker
   */
  const size_t rank;
  /**
   * @brief the global width of the image
   */
  const size_t global_width;
  /**
   * @brief the global height of the image
   */
  const size_t global_height;
  /**
   * @brief the amount of subimages in the x direction
   */
  const size_t x_tiles;
  /**
   * @brief the amount of subimages in the y direction
   */
  const size_t y_tiles;
  /**
   * @brief the current subimage in the x direction, staring from the top left
   */
  const size_t x_tile;
  /**
   * @brief the current subimage in the y direction, staring from the top left
   */
  const size_t y_tile;
  /**
   * @brief the local width of all subimages
   */
  const size_t local_width;
  /**
   * @brief the local height of all subimages
   */
  const size_t local_height;

  /**
   * @brief Constructs a new context with a given thrill context and a global width and height
   */
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

  /**
   * @brief transforms a global pixel index to a local one
   */
  inline size_t to_local_index(size_t index) const {
    return index % (local_width * local_height);
  }

  /**
   * @brief calculates the local x position of the pixel from a global index
   */
  inline size_t index_to_local_x(size_t index) const {
    return to_local_index(index) % local_width;
  }

  /**
   * @brief calculates the local y position of the pixel from a global index
   */
  inline size_t index_to_local_y(size_t index) const {
    return to_local_index(index) / local_width;
  }

  /**
   * @brief calculates the global x position of the pixel from a global index
   */
  inline size_t index_to_global_x(size_t index) const {
    return index_to_local_x(index) + x_tile * local_width;
  }

  /**
   * @brief calculates the global y position of the pixel from a global index
   */
  inline size_t index_to_global_y(size_t index) const {
    return index_to_local_y(index) + y_tile * local_height;
  }

  /**
   * @brief calculates the local x position of a pixel from a global x
   */
  inline size_t to_local_x(size_t global_x) const {
    return global_x % local_width;
  }

  /**
   * @brief calculates the local y position of a pixel from a global y
   */
  inline size_t to_local_y(size_t global_y) const {
    return global_y % local_height;
  }

  /**
   * @brief calculates the global x position of a pixel from a local x
   */
  inline size_t to_global_x(size_t local_x) const {
    return local_x + x_tile * local_width;
  }

  /**
   * @brief calculates the global y position of a pixel from a local y
   */
  inline size_t to_global_y(size_t local_y) const {
    return local_y + y_tile * local_height;
  }
};

}  // namespace model
