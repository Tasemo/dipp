#include "string_utils.hpp"

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <vector>

std::vector<std::string_view> util::split(std::string_view string, std::string_view delimiters, size_t skip_lines, char skip_char) {
  std::vector<std::string_view> substrings;
  if (delimiters.empty()) {
    substrings.emplace_back(string);
    return substrings;
  }
  auto start_pos = string.find_first_not_of(delimiters);
  auto max_length = string.length();
  size_t skipped = 0;
  while (start_pos < max_length) {
    auto end_pos = std::min(max_length, string.find_first_of(delimiters, start_pos));
    if (end_pos != start_pos) {
      if (string[start_pos] != skip_char) {
        if (skipped < skip_lines) {
          skipped++;
        } else {
          substrings.emplace_back(&string[start_pos], end_pos - start_pos);
        }
      }
      start_pos = string.find_first_not_of(delimiters, end_pos);
    }
  }
  return substrings;
}
