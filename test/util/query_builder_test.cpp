#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <util/query_builder.hpp>

TEST(QueryBuilder, BasicTest) {
  util::QueryBuilder builder;
  builder.add_part("test", 42);
  builder.add_part("test2", 43L);
  EXPECT_EQ("test=42&test2=43", builder.build());
}

TEST(QueryBuilder, EscapeTest) {
  util::QueryBuilder builder;
  builder.add_part("test", "A B");
  EXPECT_EQ("test=A%20B", builder.build());
}

TEST(QueryBuilder, StringTest) {
  using namespace std::literals;
  util::QueryBuilder builder;
  std::string string1 = "S1";
  const char* string2 = "S2";
  std::string_view string3 = "S3"sv;
  builder.add_part("S1", string1);
  builder.add_part("S2", string2);
  builder.add_part("S3", string3);
  builder.add_part("S4", "S4");
  EXPECT_EQ("S1=S1&S2=S2&S3=S3&S4=S4", builder.build());
}
