#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <util/string_utils.hpp>

TEST(ToString, BasicTest) {
  EXPECT_EQ("42", util::to_string(42));
  EXPECT_EQ("43", util::to_string(43.0));
}

TEST(ToString, StringTest) {
  using namespace std::literals;
  std::string string1 = "S1";
  const char* string2 = "S2";
  std::string_view string3 = "S3"sv;
  EXPECT_EQ("S1", util::to_string(string1));
  EXPECT_EQ("S2", util::to_string(string2));
  EXPECT_EQ("S3", util::to_string(string3));
  EXPECT_EQ("S4", util::to_string("S4"));
}
