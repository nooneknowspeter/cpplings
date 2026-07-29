#pragma once

#include "include/exercise_iterator.hpp"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace TUI
{

enum class Commands
{
    NextExercise,
    PreviousExercise,
    CompileAll,
    Refresh,
    Draw,
    Clear,
    Reset,
    Quit,
};

extern std::unique_ptr<std::queue<TUI::Commands>> p_commands_queue;

inline std::mutex mutex_state;
inline std::condition_variable cv_state;

struct State final
{
    std::atomic<bool> is_running = true;
    std::filesystem::path exercises_dir_path;
    std::vector<std::filesystem::path> list_of_exercises;
    std::filesystem::path current_exercise;
    std::atomic<std::uint8_t> current_exercise_index = 0;
    std::atomic<bool> is_current_exercise_compiling = false;
    std::atomic<bool> did_current_exercise_compile = false;
    std::vector<std::filesystem::path> completed_exercises;
    std::string current_exercise_stdout;
    std::string current_exercise_stderr;
    std::vector<std::filesystem::path> list_of_chapter_support_files;
};

extern std::unique_ptr<TUI::State> p_state;

void run(ExerciseIterator::ExerciseDirectories directory = ExerciseIterator::ExerciseDirectories::Exercises);

} // namespace TUI
