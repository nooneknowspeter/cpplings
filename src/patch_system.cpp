#include "include/patch_system.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/readable_pipe.hpp"
#include "include/ascii.hpp"
#include "include/exercise_iterator.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include "include/util.hpp"
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <print>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

void generatePatch(std::filesystem::path patch_path, std::filesystem::path original_file_path,
                   std::filesystem::path modified_file_path)
{
    std::println("diff -u {} {} --> {}{}{}\n", original_file_path.string(), modified_file_path.string(),
                 ASCII::Styles::UNDERLINE, patch_path.string(), ASCII::Styles::CLEAR_STYLE);

    boost::asio::io_context process_context;
    auto bin{boost::process::environment::find_executable("diff")};
    boost::asio::readable_pipe stdout_pipe{process_context};
    boost::process::error_code error_code;
    boost::process::process process(process_context.get_executor(), bin,
                                    {"-u", original_file_path.string(), modified_file_path.string()},
                                    boost::process::process_stdio{{}, stdout_pipe, {}});

    auto read_process_pipe{util::getProcessResult(stdout_pipe, error_code)};
    process.wait();

    auto stdout_output{read_process_pipe.get()};

    std::print("{}", ASCII::Styles::BOLD);
    if (process.exit_code() != 0)
    {
        std::println("Failed to generate patch");
        return;
    }
    else
    {
        std::ofstream file{patch_path};
        if (file.is_open())
        {
            file << stdout_output;
            file.close();
        }

        std::println("Generated patch");
    }
    std::println(" -> {}{}\n", ASCII::Styles::UNDERLINE, patch_path.string(), ASCII::Styles::CLEAR_STYLE);
}

std::unique_ptr<PatchSystem> PatchSystem::getInstance()
{
    return std::make_unique<PatchSystem>();
}

void PatchSystem::generatePatches()
{
    auto exercise_iterator_instance{ExerciseIterator::getInstance()};

    exercise_iterator_instance->scanForExercises(ExerciseIterator::ExerciseDirectories::Solutions);
    auto list_of_solutions{TUI::p_state->list_of_exercises};

    exercise_iterator_instance->scanForExercises(ExerciseIterator::ExerciseDirectories::Exercises);
    auto list_of_exercises{TUI::p_state->list_of_exercises};

    if (list_of_solutions.size() != list_of_exercises.size())
    {
        Log::fatal("number of exercises: {} and solutions: {} do not match", list_of_exercises.size(),
                   list_of_solutions.size());
        std::terminate();
    }

    for (auto i{0uz}; i < list_of_solutions.size(); ++i)
    {
        auto exercise_path{list_of_exercises.at(i)};
        auto solution_path{list_of_solutions.at(i)};
        auto patch_path{solution_path};
        patch_path.replace_extension(".patch");

        generatePatch(patch_path, solution_path, exercise_path);
    }
}

void PatchSystem::patch(std::filesystem::path exercise_file_path)
{
    try
    {
        auto solutions_dir_result{
            ExerciseIterator::getExerciseDirectory(ExerciseIterator::ExerciseDirectories::Solutions)};
        if (!solutions_dir_result)
        {
            throw std::runtime_error(solutions_dir_result.error());
        }

        auto solutions_dir{solutions_dir_result.value()};

        auto solution_file_path{solutions_dir / exercise_file_path.parent_path().filename() /
                                exercise_file_path.filename()};
        auto patch_file_path{solution_file_path.replace_extension(".patch")};

        std::vector<std::string> program_args{"-u", "-o", exercise_file_path.string(), solution_file_path.string(),
                                              patch_file_path.string()};

        auto bin{boost::process::environment::find_executable("patch")};
        boost::asio::io_context process_context{};
        boost::process::process process(process_context.get_executor(), bin, program_args);
        process.wait();

        if (process.exit_code() != 0)
        {
            throw std::runtime_error("failed to restore exercise");
        }
    }
    catch (const std::exception &e)
    {
        Log::fatal("{}", e.what());
    }
}
