#pragma once

#include <lift/escape.hpp>
#include <sstream>
#include <string_view>
#include <type_traits>

namespace util {

/**
 * @brief utility builder to construct a URL encoded query of generic key-value parts separated by '&'
 */
class QueryBuilder {
 private:
  std::ostringstream _query;
  bool _is_first = true;

 public:
  /**
   * @brief adds a new key-value part to the query
   *
   * @note the value must be a string-like object or be accepted by std::to_string
   */
  template <typename T>
  QueryBuilder& add_part(std::string_view key, const T& part) {
    std::string value;
    if constexpr (
        std::is_same_v<T, std::string> ||
        std::is_same_v<std::decay_t<T>, char*> ||
        std::is_same_v<T, const char*> ||
        std::is_same_v<T, std::string_view>
    ) {
      value = part;
    } else {
      value = std::to_string(part);
    }
    if (!_is_first) {
      _query << '&';
    }
    _query << key << '=' << lift::escape(value);
    _is_first = false;
    return *this;
  }

  /**
   * @brief constructs the final query
   */
  std::string build() const {
    return _query.str();
  }
};

}  // namespace util
