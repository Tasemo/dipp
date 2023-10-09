#pragma once

#include <charconv>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

template <class T, class... Ts>
void set_cell_values(std::vector<std::string_view>& line, size_t column, T& first, Ts&... rest) {
  std::string_view cell = line[column];
  std::from_chars(cell.begin(), cell.end(), first);
  if constexpr (sizeof...(rest) > 0) {
    set_cell_values(line, column + 1, rest...);
  }
}

namespace util {

/**
 * @brief a CSV reader that works with string_view and supports reading numeric data types
 */
class CSVReader {
 private:
  const std::vector<std::vector<std::string_view>> _lines;
  size_t _current = 0;

 public:
  explicit CSVReader(std::string_view data, bool skip_first_line = false);
  std::string_view get_cell(size_t row, size_t cell) const;
  const std::vector<std::string_view>& get_line(size_t row) const;
  const std::vector<std::vector<std::string_view>>& get_lines() const;

  template <typename... T>
  bool read_line(T&... args) {
    constexpr size_t size = sizeof...(args);
    static_assert(size > 0, "at least one argument is required");
    if (_current >= _lines.size()) {
      return false;
    }
    std::vector<std::string_view> line = _lines[_current];
    if (size != line.size()) {
      throw std::out_of_range("Line has " + std::to_string(line.size()) + " entries, " + std::to_string(size) + " requested");
    }
    set_cell_values(line, 0, args...);
    _current++;
    return true;
  }
};

}  // namespace util
