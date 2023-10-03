#include "network_utils.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>

#include "lift/http.hpp"
#include "lift/lift_status.hpp"
#include "lift/request.hpp"
#include "lift/response.hpp"

lift::response util::send_request(lift::request& request, std::string_view request_name) {
  std::cout << "Sending " << lift::http::to_string(request.method()) << " request to " << request.url() << '\n';
  auto response = request.perform();
  if (response.lift_status() != lift::lift_status::success) {
    std::cout << "Error while sending " << request_name << " request, " << lift::to_string(response.lift_status()) << '\n';
    exit(EXIT_FAILURE);
  }
  if (response.status_code() != lift::http::status_code::http_200_ok) {
    std::cout << "Invalid response from " << request_name << " request, " << lift::http::to_string(response.status_code()) << '\n';
    exit(EXIT_FAILURE);
  }
  std::cout << "Loaded " << response.data().size() << " bytes in " << response.total_time().count() << " ms\n";
  return response;
}
