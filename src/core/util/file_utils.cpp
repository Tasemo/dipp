#include "file_utils.hpp"

#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <string_view>

void util::write_to_file(const std::string& data_path, std::string_view data, bool binary) {
  auto flags = std::ios::out;
  if (binary) {
    flags |= std::ios::binary;
  }
  std::ofstream out(data_path, flags);
  out << data;
  out.close();
}

std::string util::read_from_file(const std::string& data_path) {
  std::ifstream in(data_path);
  std::stringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}
