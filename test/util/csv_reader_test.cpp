#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <util/csv_reader.hpp>

TEST(CSVReader, BasicTest) {
  std::string data =
      "#someComment\n"
      "column1,column2,column3\n"
      "#someOtherComment\n"
      "42,43,44\n";
  util::CSVReader csv(data, true);
  auto lines = csv.get_lines();
  EXPECT_EQ(lines.size(), 1);
  EXPECT_EQ(lines[0][0], "42");
  EXPECT_EQ(lines[0][1], "43");
  EXPECT_EQ(lines[0][2], "44");
}

TEST(CSVReader, ReadLineConversion) {
  std::string data = "42,43,44,45";
  util::CSVReader csv(data, false);
  int value_a = 0;
  float value_b = 0;
  double value_c = 0;
  size_t value_d = 0;
  EXPECT_TRUE(csv.read_line(value_a, value_b, value_c, value_d));
  EXPECT_FALSE(csv.read_line(value_a, value_b, value_c, value_d));
  EXPECT_EQ(value_a, 42);
  EXPECT_EQ(value_b, 43);
  EXPECT_EQ(value_c, 44);
  EXPECT_EQ(value_d, 45);
}

TEST(CSVReader, ReadLineArgumentOverflow) {
  std::string data = "42";
  util::CSVReader csv(data, false);
  int value_a = 0;
  float value_b = 0;
  EXPECT_THROW(csv.read_line(value_a, value_b), std::out_of_range);
}

TEST(CSVReader, ReadLineArgumentUnderflow) {
  std::string data = "42,43";
  util::CSVReader csv(data, false);
  int value_a = 0;
  EXPECT_THROW(csv.read_line(value_a), std::out_of_range);
}

TEST(CSVReader, Empty) {
  util::CSVReader csv("", false);
  EXPECT_THROW(csv.get_line(0), std::out_of_range);
}

TEST(CSVReader, CommentsInCells) {
  std::string data =
      "column1,column2,column3\n"
      "42,#43,#44\n";
  util::CSVReader csv(data, true);
  auto lines = csv.get_lines();
  EXPECT_EQ(lines.size(), 1);
  EXPECT_EQ(lines[0][0], "42");
  EXPECT_EQ(lines[0][1], "#43");
  EXPECT_EQ(lines[0][2], "#44");
}
