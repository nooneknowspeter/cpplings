#pragma once

#include <cstdint>
#include <filesystem>

struct FileWatcher
{
    FileWatcher() = default;
    ~FileWatcher() = default;

    FileWatcher(const FileWatcher &) = delete;
    FileWatcher(FileWatcher &&) = default;

    FileWatcher &operator=(const FileWatcher &) = delete;
    FileWatcher &operator=(FileWatcher &&) = default;

    static std::unique_ptr<FileWatcher> getInstance();

    void watch(const std::uint16_t poll_ms = 1000);

  private:
    [[maybe_unused]] std::filesystem::file_time_type m_current_exercise_prev_mod_time;
};
