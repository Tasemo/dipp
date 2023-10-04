#pragma once

#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <functional>
#include <string>
#include <thrill/api/context.hpp>

inline void run(size_t hosts, size_t worker_per_host, const std::function<void(thrill::Context&)>& job_startpoint) {
  thrill::api::MemoryConfig mem_config;
  mem_config.verbose_ = false;
  mem_config.enable_proc_profiler_ = false;
  mem_config.setup(100L * 1024 * 1024);
  thrill::api::RunLocalMock(mem_config, hosts, worker_per_host, job_startpoint);
}

class TemporaryFolder : public ::testing::Test {
 protected:
  std::string _temporary_folder;

  void SetUp() override {
    std::string folder = std::filesystem::temp_directory_path();
    _temporary_folder = folder + "/testing/";
    std::filesystem::remove_all(_temporary_folder);
    std::filesystem::create_directories(_temporary_folder);
  }
};
