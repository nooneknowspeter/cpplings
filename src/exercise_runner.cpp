#include "include/exercise_runner.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include "include/util.hpp"
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <exception>
#include <filesystem>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

struct ProcessResult
{
    std::filesystem::path exercise_path;
    std::string stdout;
    std::string stderr;
    bool did_exercise_compile;
};

std::unique_ptr<ExerciseRunner> ExerciseRunner::getInstance()
{
    return std::make_unique<ExerciseRunner>();
}

[[nodiscard]]
std::vector<std::filesystem::path> scanForExerciseSupportFiles(std::filesystem::path &exercise_path)
{
    std::vector<std::filesystem::path> list_of_chapter_support_files{};

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

            list_of_chapter_support_files.push_back(entry.path());
        }
    }
    catch (const std::exception &e)
    {
        Log::fatal("{}", e.what());
    }

    return list_of_chapter_support_files;
}

[[nodiscard("process result must be used")]]
ProcessResult t_runCompilerProcess(std::filesystem::path &exercise_path) noexcept
{
    std::vector<std::string> program_args{};
    program_args.emplace_back(exercise_path.string());

    auto list_of_chapter_support_files{scanForExerciseSupportFiles(exercise_path)};
    if (!list_of_chapter_support_files.empty())
    {
        for (auto &path : list_of_chapter_support_files)
        {
            program_args.emplace_back(path.string());
        }
    }

    program_args.insert(program_args.begin(), "c++");

    auto bin{boost::process::environment::find_executable("zig")};
    boost::asio::io_context process_context;
    boost::asio::readable_pipe stdout_pipe{process_context};
    boost::asio::readable_pipe stderr_pipe{process_context};
    boost::system::error_code error_code;

    boost::process::process process(process_context.get_executor(), bin, program_args,
                                    boost::process::process_stdio{{}, stdout_pipe, stderr_pipe});

    auto stdout_future{util::getProcessResult(stdout_pipe, error_code)};
    auto stderr_future{util::getProcessResult(stderr_pipe, error_code)};
    process.wait();

    return {
        .exercise_path = exercise_path,
        .stdout = stdout_future.get(),
        .stderr = stderr_future.get(),
        .did_exercise_compile = (process.exit_code() == 0),
    };
}

void compileExercise(std::filesystem::path &exercise_path) noexcept
{
    std::unique_lock lock(TUI::mutex_state);
    TUI::cv_state.wait_for(lock, std::chrono::milliseconds(200));

    TUI::p_state->is_current_exercise_compiling = true;
    lock.unlock();
    TUI::draw();

    auto compilation_result{t_runCompilerProcess(exercise_path)};

    lock.lock();
    TUI::p_state->current_exercise_stdout = compilation_result.stdout;
    TUI::p_state->current_exercise_stderr = compilation_result.stderr;
    TUI::p_state->did_current_exercise_compile = compilation_result.did_exercise_compile;
    TUI::p_state->is_current_exercise_compiling = false;
    TUI::cv_state.notify_all();
    TUI::draw();
}

void ExerciseRunner::compileCurrentExercise()
{
    auto current_exercise_path{TUI::p_state->current_exercise};
    compileExercise(current_exercise_path);
}

void ExerciseRunner::compileAllExercises()
{
    auto list_of_exercises{TUI::p_state->list_of_exercises};
    auto hardware_threads{std::thread::hardware_concurrency() == 0 ? 2 : std::thread::hardware_concurrency()};

    std::vector<std::filesystem::path> completed_exercises;
    completed_exercises.reserve(list_of_exercises.size());

    for (auto i{0uz}; i < list_of_exercises.size(); i += hardware_threads)
    {
        auto batch_end = std::min(i + hardware_threads, list_of_exercises.size());
        std::vector<std::future<ProcessResult>> batch;
        batch.reserve(batch_end - i);

        for (auto j{i}; j < batch_end; ++j)
        {
            batch.push_back(std::async(
                std::launch::async, [&list_of_exercises, j] { return t_runCompilerProcess(list_of_exercises.at(j)); }));
        }

        for (auto &future : batch)
        {
            auto result = future.get();
            if (result.did_exercise_compile)
            {
                completed_exercises.push_back(std::move(result.exercise_path));
            }
        }
    }

    {
        std::scoped_lock lock(TUI::mutex_state);
        TUI::p_state->completed_exercises = std::move(completed_exercises);

        for (size_t i{0uz}; i < list_of_exercises.size(); ++i)
        {
            try
            {
                if (TUI::p_state->completed_exercises.at(i) != list_of_exercises.at(i))
                {
                    throw std::out_of_range("exercises do not match");
                }
            }
            catch (...)
            {
                TUI::p_state->current_exercise_index = static_cast<std::uint8_t>(i);
                TUI::p_state->current_exercise = list_of_exercises.at(i);
                break;
            }
        }
    }

    compileCurrentExercise();
}
