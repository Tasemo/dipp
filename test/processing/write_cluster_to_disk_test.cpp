#include <gtest/gtest.h>

#include <cstddef>
#include <model/context.hpp>
#include <model/pixel.hpp>
#include <opencv2/core/matx.hpp>
#include <processing/write_clusters_to_disk.hpp>
#include <string>
#include <thrill/api/cache.hpp>
#include <thrill/api/concat_to_dia.hpp>
#include <thrill/api/context.hpp>
#include <vector>

#include "../common.hpp"

using WriteClustersToDisk = TemporaryFolder;

TEST_F(WriteClustersToDisk, BasicTest) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers() * 10, ctx.num_workers() * 10);
    auto expected = cv::Vec3b{128, 128, 128};
    std::vector<std::vector<model::Pixel>> clusters;
    clusters.push_back(generate_image(5, 5, expected, ctx.my_rank() * 10));
    clusters.push_back(generate_image(5, 5, expected, ctx.my_rank() * 10 + 5));
    auto dia = thrill::api::ConcatToDIA(ctx, clusters);
    auto write_to_test = processing::WriteClustersToDisk(_temporary_folder);
    write_to_test.process(context, dia).Cache().Execute();
    for (size_t i = 0; i < clusters.size(); i++) {
      auto image_path = _temporary_folder + std::to_string(ctx.my_rank()) + '-' + std::to_string(i) + ".jpeg";
      assert_image(image_path, 5, 5, expected);
    }
  };
  run_thrill(1, 4, start_func);
}

TEST_F(WriteClustersToDisk, DiagonalCluster) {
  auto start_func = [&](thrill::Context& ctx) {
    model::Context context(ctx, ctx.num_workers() * 3, ctx.num_workers() * 2);
    auto expected = cv::Vec3b{128, 128, 128};
    std::vector<std::vector<model::Pixel>> clusters;
    std::vector<model::Pixel> image;
    image.emplace_back(1, 0, expected);
    image.emplace_back(2, 0, expected);
    image.emplace_back(0, 1, expected);
    image.emplace_back(1, 1, expected);
    clusters.push_back(image);
    auto dia = thrill::api::ConcatToDIA(ctx, clusters);
    auto write_to_test = processing::WriteClustersToDisk(_temporary_folder);
    write_to_test.process(context, dia).Cache().Execute();
    for (size_t i = 0; i < clusters.size(); i++) {
      auto image_path = _temporary_folder + std::to_string(ctx.my_rank()) + '-' + std::to_string(i) + ".jpeg";
      auto image = assert_image(image_path, 3, 2);
    }
  };
  run_thrill(1, 1, start_func);
}
