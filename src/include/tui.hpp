#pragma once

#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

namespace TUI
{

enum class Commands
{
    NextExercise,
    PreviousExercise,
    CompileAll,
    Refresh,
    Reset,
    Quit,
};

extern std::unique_ptr<std::deque<TUI::Commands>> p_commands_queue;

struct State final
{
    std::atomic<std::string> exercises_dir_path();
    std::vector<std::string> list_of_exercises;
    std::atomic<std::string> current_exercise();
    std::atomic<std::uint8_t> current_exercise_index;
    std::atomic<bool> did_current_exercise_compile;
    std::vector<std::string> completed_exercises;
    std::atomic<std::string> current_exercise_stdout();
    std::atomic<std::string> current_exercise_stderr();
    std::atomic<std::size_t> current_exercise_prev_mod_time();
    std::atomic<std::string> current_chapter();
    std::vector<std::string> list_of_chapter_support_files;
};

extern std::unique_ptr<TUI::State> p_state;

void run();

} // namespace TUI
