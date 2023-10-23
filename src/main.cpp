#include <cstdlib>
#include <iostream>
#include <loading/sdss_image_loader.hpp>
#include <loading/sdss_validation_loader.hpp>
#include <model/context.hpp>
#include <model/distribution.hpp>
#include <model/k_means_init.hpp>
#include <model/rect.hpp>
#include <model/sdss_context.hpp>
#include <processing/estimate_clusters.hpp>
#include <processing/image_to_dia.hpp>
#include <processing/k_means/lloyd.hpp>
#include <processing/pipeline.hpp>
#include <processing/pixel_cluster_mapping.hpp>
#include <processing/pixel_worker_mapping.hpp>
#include <processing/threshold.hpp>
#include <processing/validation.hpp>
#include <processing/write_image_to_disk.hpp>
#include <thrill/api/context.hpp>
#include <tlx/cmdline_parser.hpp>
#include <tlx/counting_ptr.hpp>
#include <util/paint_bounding_boxes.hpp>
#include <util/paint_clusters.hpp>
#include <vector>

#include "model/k_means_model.hpp"

struct CommandLineArgs {
  model::Distribution distribution{model::Distribution::LLOYD};
  size_t global_width{1024};
  size_t global_height{1024};
  double start_ra{180.0};
  double start_dec{0.0};
  double scale{0.315};
  size_t cluster_count{0};
  size_t max_iteratations{10};
  double epsilon{1.0};
  model::KMeansInit init{model::KMeansInit::RANDOM};
};

bool parse_command_line(CommandLineArgs &args, int argc, const char *const *argv) {
  size_t distribution{2}, k_means_init{1};
  tlx::CmdlineParser parser;
  parser.set_description(R"(Distributed image processing pipeline with various distribution methods:
  trivial (1): generates equally sized and spaced sub-images
  lloyd (2): image segmentation with k-means clustering (lloyd`s algorithm))");
  parser.set_author("Tim Oelkers <tim.oelkers@web.de>");
  parser.add_size_t('m', "distribution", distribution, "distribution method, default: 2 (lloyd)");
  parser.add_size_t('w', "width", args.global_width, "total image width, default: 1024");
  parser.add_size_t('h', "height", args.global_height, "total image height, default: 1024");
  parser.add_double('r', "start_ra", args.start_ra, "right-ascension (ra) of the top left corner of the SDSS image, default: 180.0");
  parser.add_double('d', "start_dec", args.start_dec, "declination (dec) of the top left corner of the SDSS image, default: 0.0");
  parser.add_double('s', "scale", args.scale, "scale in arcseconds per pixel of the SDSS image, default: 0.315");
  parser.add_size_t('i', "max_iterations", args.max_iteratations, "maximum number of k-means iterations, default: 10");
  parser.add_double('e', "epsilon", args.epsilon, "desired k-means accuracy, default: 1.0");
  parser.add_size_t('y', "init_strategy", k_means_init, "k-means init strategy (1 - random, 2 - k-means++), default: 1");
  parser.add_size_t('c', "cluster_count", args.cluster_count, "k-means cluster count, 0 for estimation, default: 0");
  if (!parser.process(argc, argv)) {
    return false;
  }
  if (!model::is_valid_distribution(distribution)) {
    std::cout << "Error: parameter \"method\" is invalid!\n\n";
    parser.print_usage();
    return false;
  }
  if (!model::is_valid_k_means_init(k_means_init)) {
    std::cout << "Error: parameter \"init_strategy\" is invalid!\n\n";
    parser.print_usage();
    return false;
  }
  args.distribution = model::get_distribution(distribution);
  args.init = model::get_k_means_init(k_means_init);
  return true;
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
  processing::Validation validation(bounding_boxes);
  processing::Result result;
  if (args.distribution == model::Distribution::LLOYD) {
    processing::Threshold threshold(15);
    processing::WriteImageToDisk write_image_to_disk(image_loader.get_data_dir() + "debug/");
    processing::Lloyd k_means(args.cluster_count, args.max_iteratations, args.epsilon, args.init);
    auto k_means_chain = image_to_dia.add_next(threshold)->add_next(write_image_to_disk);
    model::KMeansModel k_means_model;
    if (args.cluster_count == 0) {
      processing::EstimateClusters estimate(11);
      auto chain = k_means_chain->add_next(estimate)->add_next(k_means);
      k_means_model = chain->process(context);
    } else {
      auto chain = k_means_chain->add_next(k_means);
      k_means_model = chain->process(context);
    }
    debug_clusters.paint(k_means_model);
    processing::PixelClusterMapping pixel_cluster_mapping(k_means_model);
    auto validation_chain = image_to_dia.add_next(pixel_cluster_mapping)->add_next(validation);
    result = validation_chain->process(context);
  } else if (args.distribution == model::Distribution::TRIVIAL) {
    processing::PixelWorkerMapping pixel_worker_mapping;
    auto validation_chain = image_to_dia.add_next(pixel_worker_mapping)->add_next(validation);
    result = validation_chain->process(context);
  }
  if (context.rank == 0) {
    std::cout << result << '\n';
  }
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
