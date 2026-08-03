#include "include/exercise_iterator.hpp"
#include "include/log.hpp"
#include "include/patch_system.hpp"
#include "include/tui.hpp"
#include <algorithm>
#include <cstdint>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <limits>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <system_error>

std::unique_ptr<ExerciseIterator> ExerciseIterator::getInstance()
{
    return std::make_unique<ExerciseIterator>();
}

std::expected<std::filesystem::path, std::string> ExerciseIterator::getExerciseDirectory(ExerciseDirectories directory)
{
    std::filesystem::path cwd{std::filesystem::current_path()};

    std::filesystem::path exercises_dir_path{cwd / "exercises"};

    if (directory == ExerciseDirectories::Solutions)
    {
        exercises_dir_path = cwd / "solutions";
    }

    if (!std::filesystem::exists(exercises_dir_path))
    {
        return std::unexpected(std::format("directory does not exist: {}", exercises_dir_path.string()));
    }

    return exercises_dir_path;
}

void ExerciseIterator::scanForExercises(ExerciseDirectories directory)
{
    try
    {
        auto exercise_dir_result{getExerciseDirectory(directory)};
        if (!exercise_dir_result)
        {
            throw std::runtime_error(exercise_dir_result.error());
        }

        auto exercise_dir{std::move(exercise_dir_result.value())};

        if (std::filesystem::is_empty(exercise_dir))
        {
            throw std::filesystem::filesystem_error("exercises dir is empty", exercise_dir, std::error_code());
        }
        else
        {
            TUI::p_state->exercises_dir_path = exercise_dir;
        }

        auto list_of_exercises =
            std::filesystem::recursive_directory_iterator(exercise_dir) |
            std::views::transform([](const std::filesystem::directory_entry &entry) { return entry.path(); }) |
            std::views::filter([](const std::filesystem::path &path) {
                if (!path.has_extension())
                {
                    return false;
                }

                if (path.extension() == std::filesystem::path(".md") ||
                    path.extension() == std::filesystem::path(".patch") ||
                    path.extension() == std::filesystem::path(".rej"))
                {
                    return false;
                }

                if (path.string().contains("src") || path.string().contains("include"))
                {
                    return false;
                }

                return true;
            }) |
            std::ranges::to<std::vector<std::filesystem::path>>();

        TUI::p_state->list_of_exercises = std::move(list_of_exercises);

        Log::info("sorting exercises");
        std::ranges::sort(TUI::p_state->list_of_exercises);

        for (auto const &i : TUI::p_state->list_of_exercises)
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

std::expected<void, std::string> ExerciseIterator::previous()
{
    using limit = std::numeric_limits<std::uint8_t>;

    if (limit::min() == TUI::p_state->current_exercise_index || TUI::p_state->completed_exercises.empty())
    {
        return std::unexpected(std::format("can't go past the first exercise"));
    }

    TUI::p_state->completed_exercises.pop_back();
    TUI::p_state->current_exercise_index--;
    TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.at(TUI::p_state->current_exercise_index);
    return {};
}

std::expected<void, std::string> ExerciseIterator::next()
{
    if (TUI::p_state->is_current_exercise_compiling)
    {
        return std::unexpected(std::format("current exercise is compiling"));
    }

    if (!TUI::p_state->did_current_exercise_compile)
    {
        return std::unexpected(std::format("current exercise failed to compile"));
    }

    if (TUI::p_state->list_of_exercises.size() == TUI::p_state->current_exercise_index)
    {
        return std::unexpected(std::format("can't go past the last exercise"));
    }

    TUI::p_state->completed_exercises.push_back(TUI::p_state->current_exercise);
    TUI::p_state->current_exercise_index++;

    if (TUI::p_state->current_exercise_index < TUI::p_state->list_of_exercises.size())
    {
        TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.at(TUI::p_state->current_exercise_index);
    }

    return {};
}

void ExerciseIterator::reset()
{
    Log::info("reset current exercise");

    PatchSystem::patch(TUI::p_state->current_exercise);
}
