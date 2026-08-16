#pragma once

#include <cstdint>
#include <filesystem>
#include <stop_token>

struct FileWatcher {
  FileWatcher() = default;
  ~FileWatcher() = default;

  FileWatcher(const FileWatcher&) = delete;
  FileWatcher(FileWatcher&&) = default;

  FileWatcher& operator=(const FileWatcher&) = delete;
  FileWatcher& operator=(FileWatcher&&) = default;

  static std::unique_ptr<FileWatcher> getInstance();

  void watch(std::stop_token s_token, const std::uint16_t poll_ms);

 private:
  [[maybe_unused]] std::filesystem::file_time_type
      m_current_exercise_prev_mod_time;
};
