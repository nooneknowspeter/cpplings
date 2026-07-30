#include "include/exercise_runner.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <filesystem>
#include <future>
#include <memory>
#include <mutex>
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

[[nodiscard("process output must be used")]]
std::string getProcessResult(boost::asio::readable_pipe &process_pipe)
{
    std::string result;

    boost::asio::streambuf buffer;
    boost::system::error_code error_code;

    while (boost::asio::read(process_pipe, buffer, error_code))
    {
        result.append(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
        buffer.consume(buffer.size()); // NOTE: flushing
    }

    return result;
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

    boost::process::process process(process_context.get_executor(), bin, program_args,
                                    boost::process::process_stdio{{}, stdout_pipe, stderr_pipe});

    auto stdout_future{std::async(std::launch::async, [&stdout_pipe] { return getProcessResult(stdout_pipe); })};
    auto stderr_future{std::async(std::launch::async, [&stderr_pipe] { return getProcessResult(stderr_pipe); })};
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

// TODO: SIMD
void ExerciseRunner::compileAllExercises()
{
}
