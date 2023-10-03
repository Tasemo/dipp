#pragma once

#include <lift/escape.hpp>
#include <sstream>
#include <string_view>
#include <util/string_utils.hpp>

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
   * @brief adds a new key-value part to the query and URL encodes the value if needed
   *
   * @note the value must be a string-like object or be numeric (accepted by std::to_string)
   */
  template <typename T>
  QueryBuilder& add_part(std::string_view key, const T& part) {
    std::string value = util::to_string(part);
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
