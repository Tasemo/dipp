#include <cstddef>
#include <string_view>
#include <util/string_utils.hpp>
#include <vector>

#include "csv_reader.hpp"

std::vector<std::vector<std::string_view>> tokenize_csv(std::string_view data, bool skip_first_line) {
  std::vector<std::string_view> lines = util::split(data, "\n", static_cast<size_t>(skip_first_line), '#');
  std::vector<std::vector<std::string_view>> result;
  result.reserve(lines.size());
  for (std::string_view line : lines) {
    result.push_back(util::split(line, ","));
  }
  return result;
}

util::CSVReader::CSVReader(std::string_view data, bool skip_first_line)
    : _lines(tokenize_csv(data, skip_first_line)) {}

std::string_view util::CSVReader::get_cell(size_t row, size_t cell) const {
  return _lines.at(row).at(cell);
}

const std::vector<std::string_view>& util::CSVReader::get_line(size_t row) const {
  return _lines.at(row);
}

const std::vector<std::vector<std::string_view>>& util::CSVReader::get_lines() const {
  return _lines;
}
