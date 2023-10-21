#include "estimate_clusters.hpp"

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <thrill/api/cache.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/net/flow_control_channel.hpp>
#include <vector>

static const model::Pixel EMPTY_PIXEL = model::Pixel(cv::Vec2d{-1, -1}, cv::Vec3b{});

void depth_search(std::vector<std::vector<model::Pixel>>& grid, int y, int x) {
  if (y < 0 || x < 0) {
    return;
  }
  auto positive_x = static_cast<size_t>(x);
  auto positive_y = static_cast<size_t>(y);
  if (positive_y >= grid.size() || positive_x >= grid[positive_y].size() || grid[positive_y][positive_x] == EMPTY_PIXEL) {
    return;
  }
  grid[positive_y][positive_x] = EMPTY_PIXEL;
  depth_search(grid, y + 1, x);
  depth_search(grid, y - 1, x);
  depth_search(grid, y, x + 1);
  depth_search(grid, y, x - 1);
}

size_t count_clusters(std::vector<std::vector<model::Pixel>>& grid) {
  size_t count = 0;
  for (size_t y = 0; y < grid.size(); y++) {
    for (size_t x = 0; x < grid[y].size(); x++) {
      if (grid[y][x] != EMPTY_PIXEL) {
        depth_search(grid, y, x);
        count++;
      }
    }
  }
  return count;
}

processing::ClusterEstimation processing::EstimateClusters::process(const model::Context& ctx, const thrill::DIA<model::Pixel>& pixels) const {
  std::vector<std::vector<model::Pixel>> grid(ctx.local_height, std::vector<model::Pixel>(ctx.local_width, EMPTY_PIXEL));
  auto to_local = pixels.Map([&ctx, &grid](const model::Pixel& p) {
    size_t local_x = ctx.to_local_x(p.location[0]);
    size_t local_y = ctx.to_local_y(p.location[1]);
    grid[local_y][local_x] = p;
    return p;
  });
  auto cached = to_local.Cache().Execute();
  auto local_count = count_clusters(grid);
  auto global_count = ctx.ctx.net.AllReduce(local_count);
  return ClusterEstimation{cached, local_count, global_count};
}
