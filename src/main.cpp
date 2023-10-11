#include <cstdlib>
#include <loading/sdss_image_loader.hpp>
#include <loading/sdss_validation_loader.hpp>
#include <model/context.hpp>
#include <model/sdss_context.hpp>
#include <processing/image_to_dia.hpp>
#include <processing/k_means.hpp>
#include <processing/threshold.hpp>
#include <processing/write_image_to_disk.hpp>
#include <thrill/api/context.hpp>
#include <tlx/cmdline_parser.hpp>
#include <util/paint_bounding_boxes.hpp>
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

bool parse_command_line(CommandLineArgs &args, int argc, const char *const *argv) {
  tlx::CmdlineParser parser;
  parser.set_description(R"(Distributed image processing pipeline with various distribution methods)");
  parser.set_author("Tim Oelkers <tim.oelkers@web.de>");
  parser.add_size_t('w', "width", args.global_width, "total image width, default: 1024");
  parser.add_size_t('h', "height", args.global_height, "total image height, default: 1024");
  parser.add_double('r', "start_ra", args.start_ra, "right-ascension (ra) of the top left corner of the SDSS image, default: 180.0");
  parser.add_double('d', "start_dec", args.start_dec, "declination (dec) of the top left corner of the SDSS image, default: 0.0");
  parser.add_double('s', "scale", args.scale, "scale in arcseconds per pixel of the SDSS image, default: 1.0");
  parser.add_size_t('i', "max_iterations", args.max_iteratations, "maximum number of k-means iterations, default: 10");
  parser.add_double('e', "epsilon", args.epsilon, "desired k-means accuracy, default: 1.0");
  return parser.process(argc, argv);
}

void process(thrill::Context &ctx, const CommandLineArgs &args) {
  model::Context context(ctx, args.global_width, args.global_height);
  model::SDSSContext sdss(context, args.start_ra, args.start_dec, args.scale);
  loading::SDSSImageLoader image_loader(sdss);
  auto image = image_loader.load_image();
  util::PaintClusters debug_clusters(context, image, image_loader.get_data_dir() + "clusters/");
  util::PaintBoundingBoxes debug_boxes(context, image, image_loader.get_data_dir() + "boxes/");

  std::vector<model::Rect> bounding_boxes;
  if (context.rank == 0) {
    loading::SDSSValidationLoader validation_loader(sdss);
    bounding_boxes = validation_loader.load_bounding_boxes(image_loader.get_data_dir() + "validation.csv");
  }
  bounding_boxes = ctx.net.Broadcast(bounding_boxes, 0);
  debug_boxes.paint(bounding_boxes);

  processing::ImageToDIA image_to_dia(image);
  processing::Threshold threshold(15);
  processing::WriteImageToDisk write_image_to_disk(image_loader.get_data_dir() + "debug/");
  processing::KMeans k_means(args.cluster_count, args.max_iteratations, args.epsilon);
  auto k_means_chain = image_to_dia.add_next(threshold)->add_next(write_image_to_disk)->add_next(k_means);
  auto k_means_model = k_means_chain->process(context);
  debug_clusters.paint(k_means_model);
}

int main(int argc, const char *const *argv) {
  CommandLineArgs args{};
  if (!parse_command_line(args, argc, argv)) {
    return EXIT_FAILURE;
  }
  return thrill::api::Run([&args](thrill::api::Context &ctx) {
    process(ctx, args);
  });
}
