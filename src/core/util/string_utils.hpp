#pragma once

#include <ios>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace util {

/**
 * @brief converts a value to a std::string
 *
 * @note the value must be a string-like object or be numeric (accepted by std::to_string)
 * @note does not print decimal values if they are zero
 */
template <typename T>
inline std::string to_string(const T& value) {
  if constexpr (
      std::is_same_v<T, std::string> ||
      std::is_same_v<std::decay_t<T>, char*> ||
      std::is_same_v<T, const char*>
  ) {
    return value;
  }
  if constexpr (std::is_same_v<T, std::string_view>) {
    return std::string{value};
  }
  std::ostringstream oss;
  oss << std::noshowpoint << value;
  return oss.str();
}

}  // namespace util
