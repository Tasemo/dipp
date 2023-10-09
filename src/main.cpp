#include <loading/sdss_image_loader.hpp>
#include <loading/sdss_validation_loader.hpp>
#include <model/context.hpp>
#include <model/sdss_context.hpp>
#include <processing/image_to_dia.hpp>
#include <processing/k_means.hpp>
#include <processing/threshold.hpp>
#include <processing/write_image_to_disk.hpp>
#include <thrill/api/context.hpp>
#include <util/paint_clusters.hpp>

struct CommandLineArgs {
  size_t global_width{1024};
  size_t global_height{1024};
  double start_ra{180.0};
  double start_dec{0.0};
  double scale{1.0};
  size_t cluster_count{6};
  size_t max_iteratations{10};
  double epsilon{1.0};
};

void process(thrill::Context &ctx, const CommandLineArgs &args) {
  model::Context context(ctx, args.global_width, args.global_height);
  model::SDSSContext sdss(context, args.start_ra, args.start_dec, args.scale);
  loading::SDSSImageLoader image_loader(sdss);

  auto image = image_loader.load_image();
  util::PaintClusters debug_clusters(context, image, image_loader.get_data_dir() + "clusters/");

  std::vector<model::Rect> bounding_boxes;
  if (context.rank == 0) {
    loading::SDSSValidationLoader validation_loader(sdss);
    bounding_boxes = validation_loader.load_bounding_boxes(image_loader.get_data_dir() + "validation.csv");
  }
  bounding_boxes = ctx.net.Broadcast(bounding_boxes, 0);

  processing::ImageToDIA image_to_dia(image);
  processing::Threshold threshold(15);
  processing::WriteImageToDisk write_image_to_disk(image_loader.get_data_dir() + "debug/");
  processing::KMeans k_means(args.cluster_count, args.max_iteratations, args.epsilon);
  auto k_means_chain = image_to_dia.add_next(threshold)->add_next(write_image_to_disk)->add_next(k_means);
  auto k_means_model = k_means_chain->process(context);
  debug_clusters.paint(k_means_model);
}

int main() {
  CommandLineArgs args{};
  return thrill::api::Run([&args](thrill::api::Context &ctx) {
    process(ctx, args);
  });
}
