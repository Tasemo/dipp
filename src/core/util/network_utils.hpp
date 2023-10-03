#pragma once

#include <lift/lift.hpp>

namespace util {

/**
 * @brief performs a HTTP request, checks the response for errors and logs relevant information
 */
lift::response send_request(lift::request& request, std::string_view request_name = "");

}  // namespace util
