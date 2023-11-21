#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <tlx/cmdline_parser.hpp>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
struct AlwaysFalse : std::false_type {};

struct BaseOption {
  virtual ~BaseOption() = default;
  virtual bool is_valid_option() const = 0;
  virtual void set_original() = 0;
  virtual std::string get_name() const = 0;
};

template <typename T>
struct Option : public BaseOption {
  T* original;
  std::underlying_type_t<T> temporary;
  std::string name;

  Option(T& original, std::underlying_type_t<T> temporary, std::string name)
      : original(&original), temporary(temporary), name(std::move(name)) {}

  bool is_valid_option() const override {
    return is_valid(*original, temporary);
  }

  void set_original() override {
    *original = static_cast<T>(temporary);
  }

  std::string get_name() const override {
    return name;
  }
};

namespace util {
/**
 * @brief extends the tlx::CmdlineParser with the abillity to parse enums
 */
class CommandLineParser {
 private:
  tlx::CmdlineParser _cmd;
  std::vector<std::unique_ptr<BaseOption>> _options;

  bool process_options();

 public:
  void set_description(const std::string& description);
  void set_author(const std::string& author);
  void set_verbose_process(bool verbose_process);
  void add_bool(char key, const std::string& longkey, bool& dest, const std::string& desc);
  void add_flag(char key, const std::string& longkey, bool& dest, const std::string& desc);
  void add_int(char key, const std::string& longkey, int& dest, const std::string& desc);
  void add_unsigned(char key, const std::string& longkey, unsigned int& dest, const std::string& desc);
  void add_uint(char key, const std::string& longkey, unsigned int& dest, const std::string& desc);
  void add_size_t(char key, const std::string& longkey, size_t& dest, const std::string& desc);
  void add_float(char key, const std::string& longkey, float& dest, const std::string& desc);
  void add_double(char key, const std::string& longkey, double& dest, const std::string& desc);
  void add_bytes(char key, const std::string& longkey, uint32_t& dest, const std::string& desc);
  void add_bytes(char key, const std::string& longkey, uint64_t& dest, const std::string& desc);
  void add_string(char key, const std::string& longkey, std::string& dest, const std::string& desc);
  void add_stringlist(char key, const std::string& longkey, std::vector<std::string>& dest, const std::string& desc);
  void add_bool(const std::string& longkey, bool& dest, const std::string& desc);
  void add_flag(const std::string& longkey, bool& dest, const std::string& desc);
  void add_int(const std::string& longkey, int& dest, const std::string& desc);
  void add_unsigned(const std::string& longkey, unsigned int& dest, const std::string& desc);
  void add_uint(const std::string& longkey, unsigned int& dest, const std::string& desc);
  void add_size_t(const std::string& longkey, size_t& dest, const std::string& desc);
  void add_float(const std::string& longkey, float& dest, const std::string& desc);
  void add_double(const std::string& longkey, double& dest, const std::string& desc);
  void add_bytes(const std::string& longkey, uint32_t& dest, const std::string& desc);
  void add_bytes(const std::string& longkey, uint64_t& dest, const std::string& desc);
  void add_string(const std::string& longkey, std::string& dest, const std::string& desc);
  void add_stringlist(const std::string& longkey, std::vector<std::string>& dest, const std::string& desc);
  void add_bool(char key, const std::string& longkey, const std::string& keytype, bool& dest, const std::string& desc);
  void add_flag(char key, const std::string& longkey, const std::string& keytype, bool& dest, const std::string& desc);
  void add_int(char key, const std::string& longkey, const std::string& keytype, int& dest, const std::string& desc);
  void add_unsigned(char key, const std::string& longkey, const std::string& keytype, unsigned int& dest, const std::string& desc);
  void add_uint(char key, const std::string& longkey, const std::string& keytype, unsigned int& dest, const std::string& desc);
  void add_size_t(char key, const std::string& longkey, const std::string& keytype, size_t& dest, const std::string& desc);
  void add_float(char key, const std::string& longkey, const std::string& keytype, float& dest, const std::string& desc);
  void add_double(char key, const std::string& longkey, const std::string& keytype, double& dest, const std::string& desc);
  void add_bytes(char key, const std::string& longkey, const std::string& keytype, uint32_t& dest, const std::string& desc);
  void add_bytes(char key, const std::string& longkey, const std::string& keytype, uint64_t& dest, const std::string& desc);
  void add_string(char key, const std::string& longkey, const std::string& keytype, std::string& dest, const std::string& desc);
  void add_stringlist(char key, const std::string& longkey, const std::string& keytype, std::vector<std::string>& dest, const std::string& desc);
  void add_param_int(const std::string& name, int& dest, const std::string& desc);
  void add_param_unsigned(const std::string& name, unsigned int& dest, const std::string& desc);
  void add_param_uint(const std::string& name, unsigned int& dest, const std::string& desc);
  void add_param_size_t(const std::string& name, size_t& dest, const std::string& desc);
  void add_param_float(const std::string& name, float& dest, const std::string& desc);
  void add_param_double(const std::string& name, double& dest, const std::string& desc);
  void add_param_bytes(const std::string& name, uint32_t& dest, const std::string& desc);
  void add_param_bytes(const std::string& name, uint64_t& dest, const std::string& desc);
  void add_param_string(const std::string& name, std::string& dest, const std::string& desc);
  void add_param_stringlist(const std::string& name, std::vector<std::string>& dest, const std::string& desc);
  void add_opt_param_int(const std::string& name, int& dest, const std::string& desc);
  void add_opt_param_unsigned(const std::string& name, unsigned int& dest, const std::string& desc);
  void add_opt_param_uint(const std::string& name, unsigned int& dest, const std::string& desc);
  void add_opt_param_size_t(const std::string& name, size_t& dest, const std::string& desc);
  void add_opt_param_float(const std::string& name, float& dest, const std::string& desc);
  void add_opt_param_double(const std::string& name, double& dest, const std::string& desc);
  void add_opt_param_bytes(const std::string& name, uint32_t& dest, const std::string& desc);
  void add_opt_param_bytes(const std::string& name, uint64_t& dest, const std::string& desc);
  void add_opt_param_string(const std::string& name, std::string& dest, const std::string& desc);
  void add_opt_param_stringlist(const std::string& name, std::vector<std::string>& dest, const std::string& desc);
  void print_usage(std::ostream& os);
  void print_usage();
  CommandLineParser& sort();
  bool process(int argc, const char* const* argv, std::ostream& os);
  bool process(int argc, const char* const* argv);
  void print_result(std::ostream& os);
  void print_result();

  /**
   * @brief
   *
   * @note destination must be an enum and a corresponding function "bool is_valid(Enum, size_t)" must exist
   */
  template <typename T>
  void add_option(char key, const std::string& longkey, T& dest, const std::string& desc) {
    static_assert(std::is_enum_v<T>, "Options only work with enum types!");
    auto underlying = static_cast<std::underlying_type_t<T>>(dest);
    auto option = std::make_unique<Option<T>>(dest, underlying, longkey);
    if constexpr (std::is_same_v<std::underlying_type_t<T>, int>) {
      _cmd.add_int(key, longkey, option->temporary, desc);
    } else if constexpr (std::is_same_v<std::underlying_type_t<T>, unsigned int>) {
      _cmd.add_unsigned(key, longkey, option->temporary, desc);
    } else if constexpr (std::is_same_v<std::underlying_type_t<T>, size_t>) {
      _cmd.add_size_t(key, longkey, option->temporary, desc);
    } else {
      static_assert(AlwaysFalse<T>::value, "Unknown underlying enum type!");
    }
    _options.push_back(std::move(option));
  }
};

}  // namespace util
