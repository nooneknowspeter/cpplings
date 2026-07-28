#include "include/exercise_runner.hpp"
#include "boost/process/v1/args.hpp"
#include "boost/process/v2/process.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <filesystem>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <vector>

std::unique_ptr<ExerciseRunner> ExerciseRunner::getInstance()
{
    return std::make_unique<ExerciseRunner>();
}

void scanForExerciseSupportFiles(std::filesystem::path &exercise_path)
{
    try
    {
        auto chapter_dir{exercise_path.parent_path()};

        if (std::filesystem::is_empty(chapter_dir))
        {
            throw std::filesystem::filesystem_error("exercise chapter dir is empty", chapter_dir, std::error_code());
        }

        for (auto &entry : std::filesystem::recursive_directory_iterator(chapter_dir))
        {
            if (!entry.path().has_extension())
            {
                continue;
            }

            if (entry.path().extension() == std::filesystem::path(".h"))
            {
                continue;
            }

            if (entry.path().extension() == std::filesystem::path(".hpp"))
            {
                continue;
            }

            if (!entry.path().string().contains("src"))
            {
                continue;
            }

            Log::info("{}", entry.path().string());

            TUI::p_state->list_of_chapter_support_files.emplace_back(entry.path());
        }
    }
    catch (const std::exception &e)
    {
        Log::fatal("{}", e.what());
    }
}

// TODO: add synchronization
void t_compile(std::filesystem::path &exercise_path) noexcept
{
    std::unique_lock lock(TUI::mutex_state);
    TUI::cv_state.wait_for(lock, std::chrono::milliseconds(200));

    TUI::p_state->is_current_exercise_compiling = true;
    TUI::p_state->render_tick++;

    // std::scoped_lock lock(TUI::mutex_state);
    std::vector<std::string> program_args{};
    program_args.emplace_back(exercise_path.string());

    scanForExerciseSupportFiles(exercise_path);
    for (auto &path : TUI::p_state->list_of_chapter_support_files)
    {
        program_args.emplace_back(path.string());
    }

    program_args.insert(program_args.begin(), "c++");

    auto bin{boost::process::environment::find_executable("zig")};
    boost::asio::io_context process_context;
    [[maybe_unused]] boost::process::process process(process_context.get_executor(), bin, program_args);

    // TODO: get process stdout and stderr

    TUI::p_state->did_current_exercise_compile = true;

    TUI::p_state->is_current_exercise_compiling = false;
    TUI::p_state->render_tick++;

    TUI::cv_state.notify_all();
}

void ExerciseRunner::compileCurrentExercise()
{
    auto current_exercise_path{TUI::p_state->current_exercise};
    t_compile(current_exercise_path);
}

// TODO: SIMD
void ExerciseRunner::compileAllExercises()
{
}
