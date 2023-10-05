#pragma once

#include <lift/response.hpp>
#include <model/sdss_context.hpp>
#include <opencv2/core/mat.hpp>
#include <string>

namespace loading {

/**
 * @brief loader responsible for SDSS images (inlcuding validation images),
 * only queries remote servers once and caches images on disk
 */
class SDSSImageLoader {
 private:
  const model::SDSSContext& _sdss;

  lift::response load_from_network(const std::string& options = "") const;
  cv::Mat load(const std::string& data_dir, const std::string& options = "") const;

 public:
  /**
   * @brief constructs a new SDSS image loader with a SDSS context
   */
  explicit SDSSImageLoader(const model::SDSSContext& sdss);
  /**
   * @brief the directory where images are stored
   */
  std::string get_data_dir() const;
  /**
   * @brief loads the image either from disk or network, decodes it and returns an image in BGR format
   */
  cv::Mat load_image();
  /**
   * @brief loads the validation image (includes bounding boxes) either from disk or network,
   * decodes it and returns an image in BGR format
   */
  cv::Mat load_validation();
};

}  // namespace loading
