#include "include/exercise_iterator.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include <algorithm>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <limits>
#include <memory>
#include <stdexcept>
#include <system_error>

std::unique_ptr<ExerciseIterator> ExerciseIterator::getInstance()
{
    return std::make_unique<ExerciseIterator>();
}

void ExerciseIterator::updateExerciseDirectory(ExerciseDirectories directory = ExerciseDirectories::Exercises)
{
    std::filesystem::path cwd{std::filesystem::current_path()};

    try
    {
        std::filesystem::path exercises_dir_path{cwd / "exercises"};

        if (directory == ExerciseDirectories::Solutions)
        {
            exercises_dir_path = cwd / "solutions";
        }

        if (!std::filesystem::exists(exercises_dir_path))
        {
            throw std::filesystem::filesystem_error("exercises directory does not exist", exercises_dir_path,
                                                    std::error_code());
        }

        TUI::p_state->exercises_dir_path = exercises_dir_path;
    }
    catch (const std::exception &e)
    {
        Log::fatal("{}", e.what());
    }
    catch (...)
    {
        Log::fatal("unknown error");
    }
}

void ExerciseIterator::scanForExercises()
{
    try
    {
        if (std::filesystem::is_empty(TUI::p_state->exercises_dir_path))
        {
            throw std::filesystem::filesystem_error("exercises dir is empty", TUI::p_state->exercises_dir_path,
                                                    std::error_code());
        }

        for (auto &entry : std::filesystem::recursive_directory_iterator(TUI::p_state->exercises_dir_path))
        {
            if (!entry.path().has_extension())
            {
                continue;
            }

            if (entry.path().extension() == std::filesystem::path(".md"))
            {
                continue;
            }

            if (entry.path().string().contains("src") || entry.path().string().contains("include"))
            {
                continue;
            }

            Log::info("{}", entry.path().string());

            TUI::p_state->list_of_exercises.emplace_back(entry.path());
        }

        Log::info("sorting exercises");
        std::sort(TUI::p_state->list_of_exercises.begin(), TUI::p_state->list_of_exercises.end());

        for (auto &i : TUI::p_state->list_of_exercises)
        {
            Log::info("{}", i.string());
        }

        Log::info("set current exercise");
        TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.at(0);
    }
    catch (const std::exception &e)
    {
        Log::fatal("{}", e.what());
    }
}

void ExerciseIterator::previous()
{
    using limit = std::numeric_limits<std::uint8_t>;

    try
    {
        if (limit::min() == TUI::p_state->current_exercise_index || TUI::p_state->completed_exercises.empty())
        {
            throw std::runtime_error(std::format("can't go past the first exercise"));
        }

        TUI::p_state->completed_exercises.pop_back();
        TUI::p_state->current_exercise_index--;
        TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.at(TUI::p_state->current_exercise_index);
    }
    catch (const std::exception &e)
    {
        Log::warning("{}", e.what());

        return;
    }
}

void ExerciseIterator::next()
{
    try
    {
        if (TUI::p_state->list_of_exercises.size() == TUI::p_state->current_exercise_index)
        {
            throw std::runtime_error(std::format("can't go past the last exercise"));
        }

        if (TUI::p_state->is_current_exercise_compiling)
        {
            throw std::runtime_error(std::format("current exercise is compiling"));
        }

        if (!TUI::p_state->did_current_exercise_compile)
        {
            throw std::runtime_error(std::format("current exercise failed to compile"));
        }

        TUI::p_state->completed_exercises.push_back(TUI::p_state->current_exercise);
        TUI::p_state->current_exercise_index++;
        TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.at(TUI::p_state->current_exercise_index);
    }
    catch (const std::exception &e)
    {
        Log::warning("{}", e.what());

        return;
    }
}

void ExerciseIterator::reset()
{
    Log::info("reset current exercise");
}
