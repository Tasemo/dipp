#pragma once

#include <lift/lift.hpp>

namespace util {

/**
 * @brief performs a HTTP request, checks the response for errors and logs relevant information
 *
 * @param request the request to be send
 * @param request_name a name to identify the request in logs
 * @return lift::response the response object
 */
lift::response send_request(lift::request& request, std::string_view request_name = "");

}  // namespace util
