#pragma once

#include <charconv>
#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <util/string_utils.hpp>
#include <vector>

inline std::vector<std::vector<std::string_view>> tokenize_csv(std::string_view data, bool skip_first_line) {
  std::vector<std::string_view> lines = util::split(data, "\n", static_cast<size_t>(skip_first_line), '#');
  std::vector<std::vector<std::string_view>> result;
  result.reserve(lines.size());
  for (std::string_view line : lines) {
    result.push_back(util::split(line, ","));
  }
  return result;
}

template <class T, class... Ts>
void set_cell_values(std::vector<std::string_view>& line, size_t column, T& first, Ts&... rest) {
  std::string_view cell = line[column];
  std::from_chars(cell.begin(), cell.end(), first);
  if constexpr (sizeof...(rest) > 0) {
    set_cell_values(line, column + 1, rest...);
  }
}

namespace util {

class CSV_Reader {
 private:
  const std::vector<std::vector<std::string_view>> lines;
  size_t current = 0;

 public:
  explicit CSV_Reader(std::string_view data, bool skip_first_line = false)
      : lines(tokenize_csv(data, skip_first_line)) {}

  template <typename... T>
  bool read_line(T&... args) {
    constexpr size_t size = sizeof...(args);
    static_assert(size > 0, "at least one argument is required");
    if (current >= lines.size()) {
      return false;
    }
    std::vector<std::string_view> line = lines[current];
    if (size != line.size()) {
      throw std::out_of_range("Line has " + std::to_string(line.size()) + " entries, " + std::to_string(size) + " requested");
    }
    set_cell_values(line, 0, args...);
    current++;
    return true;
  }

  std::string_view get_cell(size_t row, size_t cell) const {
    return lines.at(row).at(cell);
  }

  const std::vector<std::string_view>& get_line(size_t row) const {
    return lines.at(row);
  }

  const std::vector<std::vector<std::string_view>>& get_lines() const {
    return lines;
  }
};

}  // namespace util
