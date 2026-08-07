#include "include/file_watcher.hpp"

#include <chrono>
#include <filesystem>
#include <jal.hpp>
#include <memory>
#include <mutex>
#include <stop_token>
#include <thread>

#include "include/tui.hpp"

using namespace jal;

std::unique_ptr<FileWatcher> FileWatcher::getInstance() {
  return std::make_unique<FileWatcher>();
}

void FileWatcher::watch(std::stop_token s_token, const std::uint16_t poll_ms) {
  while (!s_token.stop_requested()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));

    std::filesystem::path current_exercise;
    {
      std::scoped_lock lock(TUI::mutex_state);
      current_exercise = TUI::p_state->current_exercise;
    }

    if (std::filesystem::exists(current_exercise)) {
      auto file_time{std::filesystem::last_write_time(current_exercise)};

      if (this->m_current_exercise_prev_mod_time != file_time) {
        Log::info("file modified, refreshing...");
        std::scoped_lock lock(TUI::mutex_state);
        TUI::p_commands_queue->push(TUI::Commands::Refresh);

        TUI::cv_state.notify_one();
      }

      this->m_current_exercise_prev_mod_time = file_time;
    }
  }
}
