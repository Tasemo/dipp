#pragma once

#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace util {

/**
 * @brief converts a value to a std::string
 *
 * @note the value must be a string-like object or be numeric (accepted by std::to_string)
 * @note string does not contain numerical decimal values if they are zero
 */
template <typename T>
inline std::string to_string(const T& value) {
  if constexpr (std::is_same_v<T, std::string>) {
    return value;
  }
  if constexpr (
      std::is_same_v<T, std::string_view> ||
      std::is_same_v<std::decay_t<T>, char*> ||
      std::is_same_v<T, const char*>
  ) {
    return std::string(value);
  }
  std::ostringstream oss;
  oss << std::noshowpoint << value;
  return oss.str();
}

/**
 * @brief splits the given string with the given delimiters
 *
 * @note - this does not allocate new memory for substrings, so keep the original one in scope
 *
 * @param string the string to be split
 * @param delimiters one or more delimetters to split on, not included in result
 * @param skip_lines the amount of valid (not skipped by any other condition) lines to skip
 * @param skip_char skips line if it starts with this character
 * @return std::vector<std::string_view> a vector containing each substring
 */
std::vector<std::string_view> split(std::string_view string, std::string_view delimiters, size_t skip_lines = 0, char skip_char = '\0');

}  // namespace util
