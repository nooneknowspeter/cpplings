#include "include/exercise_runner.hpp"

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <future>
#include <jal.hpp>
#include <memory>
#include <mutex>
#include <ranges>
#include <stdexcept>
#include <thread>
#include <vector>

#include "include/tui.hpp"
#include "include/util.hpp"

using namespace jal;

struct ProcessResult {
  std::filesystem::path exercise_path;
  std::string stdout;
  std::string stderr;
  bool did_exercise_compile;
};

std::unique_ptr<ExerciseRunner> ExerciseRunner::getInstance() {
  return std::make_unique<ExerciseRunner>();
}

[[nodiscard]]
std::expected<std::vector<std::filesystem::path>, std::string>
scanForExerciseSupportFiles(const std::filesystem::path& exercise_path) {
  auto chapter_dir{exercise_path.parent_path()};

  if (!std::filesystem::exists(chapter_dir)) {
    return std::unexpected(std::format(
        "exercise chapter dir does not exist: {}", chapter_dir.string()));
  }

  if (std::filesystem::is_empty(chapter_dir)) {
    return std::unexpected(
        std::format("exercise chapter dir is empty: {}", chapter_dir.string()));
  }

  auto list_of_chapter_support_files =
      std::filesystem::recursive_directory_iterator(chapter_dir) |
      std::views::transform([](const std::filesystem::directory_entry& entry) {
        return entry.path();
      }) |
      std::views::filter([](const std::filesystem::path& path) {
        if (!path.has_extension()) {
          return false;
        }

        if (path.extension() == std::filesystem::path(".h") ||
            path.extension() == std::filesystem::path(".hpp")) {
          return false;
        }

        if (!path.string().contains("src")) {
          return false;
        }

        return true;
      }) |
      std::ranges::to<std::vector<std::filesystem::path>>();

  for (auto const& path : list_of_chapter_support_files) {
    Log::info("{}", path.string());
  }

  return list_of_chapter_support_files;
}

[[nodiscard("process result must be used")]]
std::expected<ProcessResult, std::string> t_runCompilerProcess(
    const std::filesystem::path& exercise_path) noexcept {
  auto relative_exercise_path{std::filesystem::relative(
      exercise_path, std::filesystem::current_path())};

  std::vector<std::string> program_args{"build", "exercises", "--"};
  program_args.emplace_back(relative_exercise_path.string());

  auto list_of_chapter_support_files{
      scanForExerciseSupportFiles(exercise_path)};
  if (!list_of_chapter_support_files) {
    return std::unexpected(list_of_chapter_support_files.error());
  }

  for (auto const& path : list_of_chapter_support_files.value()) {
    auto relative_path{
        std::filesystem::relative(path, std::filesystem::current_path())};
    program_args.emplace_back(relative_path.string());
  }

  auto bin{boost::process::environment::find_executable("zig")};
  if (bin.empty()) {
    return std::unexpected("could not find zig executable in PATH");
  }

  boost::asio::io_context process_context;
  boost::asio::readable_pipe stdout_pipe{process_context};
  boost::asio::readable_pipe stderr_pipe{process_context};
  boost::system::error_code error_code;

  boost::process::process process(
      process_context.get_executor(), bin, program_args,
      boost::process::process_stdio{{}, stdout_pipe, stderr_pipe});

  auto stdout_future{util::getProcessResult(stdout_pipe, error_code)};
  auto stderr_future{util::getProcessResult(stderr_pipe, error_code)};
  process.wait();

  return ProcessResult{
      .exercise_path = exercise_path,
      .stdout = stdout_future.get(),
      .stderr = stderr_future.get(),
      .did_exercise_compile = (process.exit_code() == 0),
  };
}

void compileExercise(const std::filesystem::path& exercise_path) noexcept {
  std::unique_lock lock(TUI::mutex_state);
  TUI::cv_state.wait_for(lock, std::chrono::milliseconds(200));

  TUI::p_state->is_current_exercise_compiling = true;
  lock.unlock();
  TUI::draw();

  auto compilation_result{t_runCompilerProcess(exercise_path)};

  lock.lock();

  if (!compilation_result) {
    TUI::p_state->current_exercise_stdout = "";
    TUI::p_state->current_exercise_stderr = compilation_result.error();
    TUI::p_state->did_current_exercise_compile = false;
  } else {
    auto const& ok{compilation_result.value()};
    TUI::p_state->current_exercise_stdout = ok.stdout;
    TUI::p_state->current_exercise_stderr = ok.stderr;
    TUI::p_state->did_current_exercise_compile = ok.did_exercise_compile;
  }

  TUI::p_state->is_current_exercise_compiling = false;
  TUI::cv_state.notify_all();
  TUI::draw();
}

void ExerciseRunner::compileCurrentExercise() {
  auto current_exercise_path{TUI::p_state->current_exercise};
  compileExercise(current_exercise_path);
}

void ExerciseRunner::compileAllExercises() {
  auto list_of_exercises{TUI::p_state->list_of_exercises};
  auto hardware_threads{std::thread::hardware_concurrency() == 0
                            ? 2
                            : std::thread::hardware_concurrency()};

  std::vector<std::filesystem::path> completed_exercises;
  completed_exercises.reserve(list_of_exercises.size());

  for (std::size_t batch_start{0}; batch_start < list_of_exercises.size();
       batch_start += hardware_threads) {
    auto batch_end{
        std::min(batch_start + hardware_threads, list_of_exercises.size())};

    std::vector<std::future<std::expected<ProcessResult, std::string>>>
        batch_futures;
    batch_futures.reserve(batch_end - batch_start);

    for (std::size_t i{batch_start}; i < batch_end; ++i) {
      auto& exercise{list_of_exercises.at(i)};
      batch_futures.push_back(std::async(std::launch::async, [&exercise] {
        return t_runCompilerProcess(exercise);
      }));
    }

    for (auto& future : batch_futures) {
      auto result{future.get()};
      if (!result) {
        Log::warning("{}", result.error());
        continue;
      }

      auto& ok{result.value()};
      if (ok.did_exercise_compile) {
        completed_exercises.push_back(std::move(ok.exercise_path));
      }
    }
  }

  {
    std::scoped_lock lock(TUI::mutex_state);
    TUI::p_state->completed_exercises = std::move(completed_exercises);

    for (auto [index, exercise] :
         std::views::zip(std::views::iota(std::size_t{0}), list_of_exercises)) {
      if (index >= TUI::p_state->completed_exercises.size() ||
          TUI::p_state->completed_exercises.at(index) != exercise) {
        TUI::p_state->current_exercise_index = static_cast<std::uint8_t>(index);
        TUI::p_state->current_exercise = exercise;
        break;
      }
    }
  }

  compileCurrentExercise();
}
