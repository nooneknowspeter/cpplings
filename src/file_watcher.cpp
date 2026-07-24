#include "include/file_watcher.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include <chrono>
#include <filesystem>
#include <memory>
#include <thread>

FileWatcher &FileWatcher::getInstance()
{
    static FileWatcher instance;

    return instance;
}

void watch(const std::uint16_t poll_ms)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));

        if (std::filesystem::exists(TUI::p_state->current_exercise))
        {

            [[maybe_unused]] auto file_time{std::filesystem::last_write_time(TUI::p_state->current_exercise)};

            if (this->m_current_exercise_prev_mod_time != file_time)
            {
                Log::info("file modified, refreshing...");
                TUI::p_commands_queue->push(TUI::Commands::Refresh);
            }

            this->m_current_exercise_prev_mod_time = file_time;
        }
    }
}
