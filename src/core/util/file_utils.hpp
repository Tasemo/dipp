#pragma once

#include <string>
#include <string_view>

namespace util {

void write_to_file(const std::string& data_path, std::string_view data, bool binary = false);
std::string read_from_file(const std::string& data_path);

}  // namespace util
