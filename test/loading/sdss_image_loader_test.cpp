#include <gtest/gtest.h>

#include <loading/sdss_image_loader.hpp>

TEST(SDSSImageLoader, BasicTest) {
  loading::SDSSImageLoader loader;
  auto image_path = loader.load_image();
  EXPECT_EQ(image_path, "/data");
}
