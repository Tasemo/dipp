#include "command_line_parser.hpp"

#include <cstddef>
#include <iostream>
#include <string>

void util::CommandLineParser::set_description(const std::string& description) {
  _cmd.set_description(description);
}

void util::CommandLineParser::set_author(const std::string& author) {
  _cmd.set_author(author);
}

void util::CommandLineParser::set_verbose_process(bool verbose_process) {
  _cmd.set_verbose_process(verbose_process);
}

void util::CommandLineParser::add_bool(char key, const std::string& longkey, bool& dest, const std::string& desc) {
  _cmd.add_bool(key, longkey, dest, desc);
}

void util::CommandLineParser::add_flag(char key, const std::string& longkey, bool& dest, const std::string& desc) {
  _cmd.add_flag(key, longkey, dest, desc);
}

void util::CommandLineParser::add_int(char key, const std::string& longkey, int& dest, const std::string& desc) {
  _cmd.add_int(key, longkey, dest, desc);
}

void util::CommandLineParser::add_unsigned(char key, const std::string& longkey, unsigned int& dest, const std::string& desc) {
  _cmd.add_unsigned(key, longkey, dest, desc);
}

void util::CommandLineParser::add_uint(char key, const std::string& longkey, unsigned int& dest, const std::string& desc) {
  _cmd.add_uint(key, longkey, dest, desc);
}

void util::CommandLineParser::add_size_t(char key, const std::string& longkey, size_t& dest, const std::string& desc) {
  _cmd.add_size_t(key, longkey, dest, desc);
}

void util::CommandLineParser::add_float(char key, const std::string& longkey, float& dest, const std::string& desc) {
  _cmd.add_float(key, longkey, dest, desc);
}

void util::CommandLineParser::add_double(char key, const std::string& longkey, double& dest, const std::string& desc) {
  _cmd.add_double(key, longkey, dest, desc);
}

void util::CommandLineParser::add_bytes(char key, const std::string& longkey, uint32_t& dest, const std::string& desc) {
  _cmd.add_bytes(key, longkey, dest, desc);
}

void util::CommandLineParser::add_bytes(char key, const std::string& longkey, uint64_t& dest, const std::string& desc) {
  _cmd.add_bytes(key, longkey, dest, desc);
}

void util::CommandLineParser::add_string(char key, const std::string& longkey, std::string& dest, const std::string& desc) {
  _cmd.add_string(key, longkey, dest, desc);
}

void util::CommandLineParser::add_stringlist(char key, const std::string& longkey, std::vector<std::string>& dest, const std::string& desc) {
  _cmd.add_stringlist(key, longkey, dest, desc);
}

void util::CommandLineParser::add_bool(const std::string& longkey, bool& dest, const std::string& desc) {
  _cmd.add_bool(longkey, dest, desc);
}

void util::CommandLineParser::add_flag(const std::string& longkey, bool& dest, const std::string& desc) {
  _cmd.add_flag(longkey, dest, desc);
}

void util::CommandLineParser::add_int(const std::string& longkey, int& dest, const std::string& desc) {
  _cmd.add_int(longkey, dest, desc);
}

void util::CommandLineParser::add_unsigned(const std::string& longkey, unsigned int& dest, const std::string& desc) {
  _cmd.add_unsigned(longkey, dest, desc);
}

void util::CommandLineParser::add_uint(const std::string& longkey, unsigned int& dest, const std::string& desc) {
  _cmd.add_uint(longkey, dest, desc);
}

void util::CommandLineParser::add_size_t(const std::string& longkey, size_t& dest, const std::string& desc) {
  _cmd.add_size_t(longkey, dest, desc);
}

void util::CommandLineParser::add_float(const std::string& longkey, float& dest, const std::string& desc) {
  _cmd.add_float(longkey, dest, desc);
}

void util::CommandLineParser::add_double(const std::string& longkey, double& dest, const std::string& desc) {
  _cmd.add_double(longkey, dest, desc);
}

void util::CommandLineParser::add_bytes(const std::string& longkey, uint32_t& dest, const std::string& desc) {
  _cmd.add_bytes(longkey, dest, desc);
}

void util::CommandLineParser::add_bytes(const std::string& longkey, uint64_t& dest, const std::string& desc) {
  _cmd.add_bytes(longkey, dest, desc);
}

void util::CommandLineParser::add_string(const std::string& longkey, std::string& dest, const std::string& desc) {
  _cmd.add_string(longkey, dest, desc);
}

void util::CommandLineParser::add_stringlist(const std::string& longkey, std::vector<std::string>& dest, const std::string& desc) {
  _cmd.add_stringlist(longkey, dest, desc);
}

void util::CommandLineParser::add_bool(char key, const std::string& longkey, const std::string& keytype, bool& dest, const std::string& desc) {
  _cmd.add_bool(key, longkey, keytype, dest, desc);
  ;
}

void util::CommandLineParser::add_flag(char key, const std::string& longkey, const std::string& keytype, bool& dest, const std::string& desc) {
  _cmd.add_flag(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_int(char key, const std::string& longkey, const std::string& keytype, int& dest, const std::string& desc) {
  _cmd.add_int(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_unsigned(char key, const std::string& longkey, const std::string& keytype, unsigned int& dest, const std::string& desc) {
  _cmd.add_unsigned(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_uint(char key, const std::string& longkey, const std::string& keytype, unsigned int& dest, const std::string& desc) {
  _cmd.add_uint(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_size_t(char key, const std::string& longkey, const std::string& keytype, size_t& dest, const std::string& desc) {
  _cmd.add_size_t(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_float(char key, const std::string& longkey, const std::string& keytype, float& dest, const std::string& desc) {
  _cmd.add_float(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_double(char key, const std::string& longkey, const std::string& keytype, double& dest, const std::string& desc) {
  _cmd.add_double(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_bytes(char key, const std::string& longkey, const std::string& keytype, uint32_t& dest, const std::string& desc) {
  _cmd.add_bytes(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_bytes(char key, const std::string& longkey, const std::string& keytype, uint64_t& dest, const std::string& desc) {
  _cmd.add_bytes(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_string(char key, const std::string& longkey, const std::string& keytype, std::string& dest, const std::string& desc) {
  _cmd.add_string(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_stringlist(char key, const std::string& longkey, const std::string& keytype, std::vector<std::string>& dest, const std::string& desc) {
  _cmd.add_stringlist(key, longkey, keytype, dest, desc);
}

void util::CommandLineParser::add_param_int(const std::string& name, int& dest, const std::string& desc) {
  _cmd.add_param_int(name, dest, desc);
}

void util::CommandLineParser::add_param_unsigned(const std::string& name, unsigned int& dest, const std::string& desc) {
  _cmd.add_param_unsigned(name, dest, desc);
}

void util::CommandLineParser::add_param_uint(const std::string& name, unsigned int& dest, const std::string& desc) {
  _cmd.add_param_uint(name, dest, desc);
}

void util::CommandLineParser::add_param_size_t(const std::string& name, size_t& dest, const std::string& desc) {
  _cmd.add_param_size_t(name, dest, desc);
}

void util::CommandLineParser::add_param_float(const std::string& name, float& dest, const std::string& desc) {
  _cmd.add_param_float(name, dest, desc);
}

void util::CommandLineParser::add_param_double(const std::string& name, double& dest, const std::string& desc) {
  _cmd.add_param_double(name, dest, desc);
}

void util::CommandLineParser::add_param_bytes(const std::string& name, uint32_t& dest, const std::string& desc) {
  _cmd.add_param_bytes(name, dest, desc);
}

void util::CommandLineParser::add_param_bytes(const std::string& name, uint64_t& dest, const std::string& desc) {
  _cmd.add_param_bytes(name, dest, desc);
}

void util::CommandLineParser::add_param_string(const std::string& name, std::string& dest, const std::string& desc) {
  _cmd.add_param_string(name, dest, desc);
}

void util::CommandLineParser::add_param_stringlist(const std::string& name, std::vector<std::string>& dest, const std::string& desc) {
  _cmd.add_param_stringlist(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_int(const std::string& name, int& dest, const std::string& desc) {
  _cmd.add_opt_param_int(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_unsigned(const std::string& name, unsigned int& dest, const std::string& desc) {
  _cmd.add_opt_param_unsigned(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_uint(const std::string& name, unsigned int& dest, const std::string& desc) {
  _cmd.add_opt_param_uint(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_size_t(const std::string& name, size_t& dest, const std::string& desc) {
  _cmd.add_opt_param_size_t(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_float(const std::string& name, float& dest, const std::string& desc) {
  _cmd.add_opt_param_float(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_double(const std::string& name, double& dest, const std::string& desc) {
  _cmd.add_opt_param_double(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_bytes(const std::string& name, uint32_t& dest, const std::string& desc) {
  _cmd.add_opt_param_bytes(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_bytes(const std::string& name, uint64_t& dest, const std::string& desc) {
  _cmd.add_opt_param_bytes(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_string(const std::string& name, std::string& dest, const std::string& desc) {
  _cmd.add_opt_param_string(name, dest, desc);
}

void util::CommandLineParser::add_opt_param_stringlist(const std::string& name, std::vector<std::string>& dest, const std::string& desc) {
  _cmd.add_opt_param_stringlist(name, dest, desc);
}

void util::CommandLineParser::print_usage(std::ostream& os) {
  _cmd.print_usage(os);
}

void util::CommandLineParser::print_usage() {
  _cmd.print_usage();
}

util::CommandLineParser& util::CommandLineParser::sort() {
  _cmd.sort();
  return *this;
}

bool util::CommandLineParser::process_options() {
  for (const auto& option : _options) {
    if (!option->is_valid_option()) {
      std::cout << "Error: parameter \"" << option->get_name() << "\" is invalid!\n\n";
      print_usage();
      _options.clear();
      return false;
    } else {
      option->set_original();
    }
  }
  _options.clear();
  return true;
}

bool util::CommandLineParser::process(int argc, const char* const* argv, std::ostream& os) {
  return _cmd.process(argc, argv, os) && process_options();
}

bool util::CommandLineParser::process(int argc, const char* const* argv) {
  return _cmd.process(argc, argv) && process_options();
}

void util::CommandLineParser::print_result(std::ostream& os) {
  _cmd.print_result(os);
}

void util::CommandLineParser::print_result() {
  _cmd.print_result();
}
