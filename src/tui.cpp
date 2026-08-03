#include "include/tui.hpp"
#include "include/ascii.hpp"
#include "include/exercise_iterator.hpp"
#include "include/exercise_runner.hpp"
#include "include/file_watcher.hpp"
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <include/log.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <print>
#include <ranges>
#include <stop_token>
#include <thread>
#include <unordered_map>

namespace TUI
{

std::unique_ptr<std::queue<TUI::Commands>> p_commands_queue;

std::unique_ptr<TUI::State> p_state;

} // namespace TUI

namespace Input
{

std::unordered_map<std::string, TUI::Commands> command_map{
    {"n", TUI::Commands::NextExercise},  {"p", TUI::Commands::PreviousExercise},
    {"c", TUI::Commands::CompileAll},    {"r", TUI::Commands::Refresh},
    {"x", TUI::Commands::Reset},         {"q", TUI::Commands::Quit},
    {"l", TUI::Commands::ListExercises}, {"h", TUI::Commands::ShowHint},
};

static void userInput(std::stop_token s_token)
{
    while (!s_token.stop_requested())
    {
        std::string input;
        std::getline(std::cin, input);

        auto map_iterator{Input::command_map.find(input)};
        if (map_iterator != command_map.end())
        {
            Log::info("{}", map_iterator->first);
            std::scoped_lock lock(TUI::mutex_state);
            TUI::p_commands_queue->push(map_iterator->second);
            TUI::cv_state.notify_one();
        }
    }
}

}; // namespace Input

struct Render
{
    static size_t tick;
    static std::uint8_t fps;
    static bool showCurrentExerciseHint;
    static bool showExercisesList;

    static void run(std::stop_token s_token)
    {
        while (!s_token.stop_requested())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps)); // NOTE: 60fps
            draw();
        }
    }

    static void draw()
    {
        Log::info("clearing terminal");
        Render::clear();

        Log::info("tick: {}", ++tick);
        Log::info("drawing");
        std::println("{}", ASCII::ART);

        if (showExercisesList)
        {
            exerciseList();
        }

        if (showCurrentExerciseHint)
        {
            hint();
        }

        std::println("{}\n{}", TUI::p_state->current_exercise_stdout, TUI::p_state->current_exercise_stderr);

        Render::compileStatus();

        Render::progress();

        Render::help();
    }

    static void clear()
    {
        std::print("{}", ASCII::Cursor::Clear::ALL);
        std::print("{}", ASCII::Cursor::Clear::SCROLLBACK);
        std::print("{}", ASCII::Cursor::Position::HOME);
    }

    static void welcome()
    {
        std::println("{}", ASCII::WELCOME);
    }

  private:
    static void exerciseList()
    {
        auto list_of_exercises{TUI::p_state->list_of_exercises};
        auto completed_exercises{TUI::p_state->completed_exercises};

        for (auto [index, exercise] : std::views::zip(std::views::iota(std::size_t{0}), list_of_exercises))
        {
            std::print("{} -> ", exercise.stem().string());

            std::print("{}", ASCII::Styles::BOLD);
            if (index < completed_exercises.size() && completed_exercises.at(index) == exercise)
            {
                std::print("Completed");
            }
            else
            {
                std::print("Not Completed");
            }

            std::println("{}", ASCII::Styles::CLEAR_STYLE);
        }
    }

    static void hint()
    {
    }

    static void compileStatus()
    {
        std::println("{}", ASCII::Styles::BOLD);
        if (TUI::p_state->is_current_exercise_compiling)
        {
            std::println("Compiling...");
            return;
        }

        if (TUI::p_state->did_current_exercise_compile)
        {
            std::print("Exercise completed, move on to the next...");
        }
        else
        {
            std::print("Exercise failed to compile. Keep trying, we believe in you.");
        }
        std::println("{}", ASCII::Styles::CLEAR_STYLE);
    }

    static void progress()
    {
        auto list_of_exercises{TUI::p_state->list_of_exercises};
        auto completed_exercises{TUI::p_state->completed_exercises};

        // NOTE: how it should look like
        // std::println("Progress: [{}] {}/{}", ascii_bar, TUI::p_state->current_exercise_index,
        // TUI::p_state->list_of_exercises.size());
        // "Progress: [{}] {}/{}" -> "Progress: [######>-----] 6/7"
        std::print("Progress: [");

        for (auto [index, exercise] : std::views::zip(std::views::iota(std::size_t{0}), list_of_exercises))
        {
            if (index == static_cast<std::size_t>(TUI::p_state->current_exercise_index.load()))
            {
                std::print(">");
                continue;
            }

            if (index < completed_exercises.size() && completed_exercises.at(index) == exercise)
            {
                std::print("#");
                continue;
            }

            std::print("-");
        }

        std::println("] {}/{}", TUI::p_state->current_exercise_index.load(), TUI::p_state->list_of_exercises.size());

        std::println("Current exercise: {1}{2}{0}{3}", TUI::p_state->current_exercise.string(), ASCII::Styles::BOLD,
                     ASCII::Styles::UNDERLINE, ASCII::Styles::CLEAR_STYLE);
    }

    static void help()
    {
        std::print("{}n{}: next / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}p{}: previous / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}c{}: check all / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}r{}: refresh / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}x{}: reset / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        // std::print("{}h{}: hint / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE); // TODO:
        std::print("{}l{}: list / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}q{}: quit ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("-> ");
    }
};

auto Render::tick{0uz};
auto Render::fps{static_cast<std::uint8_t>(5u)};
auto Render::showCurrentExerciseHint{false};
auto Render::showExercisesList{false};

namespace Runner
{

void run(ExerciseIterator::ExerciseDirectories directory)
{
    // bootstrap
    Render::clear();
    auto exercise_iterator_instance{ExerciseIterator::getInstance()};
    exercise_iterator_instance->scanForExercises(directory);

    if (directory != ExerciseIterator::ExerciseDirectories::Solutions)
    {
        if (std::filesystem::exists(std::filesystem::current_path() / ".cpplings"))
        {
            TUI::p_state->new_user = false;
        }

        if (TUI::p_state->new_user.load())
        {
            Render::welcome();
            std::string input;
            std::getline(std::cin, input);
            std::filesystem::create_directory(std::filesystem::current_path() / ".cpplings");
            TUI::p_state->new_user = false;
        }
    }

    auto exercise_runner_instance{ExerciseRunner::getInstance()};
    TUI::p_commands_queue->push(TUI::Commands::CompileAll);

    std::stop_source stop_src;
    std::stop_token s_token{stop_src.get_token()};
    auto file_watcher_thread = std::jthread([&s_token] { FileWatcher::getInstance()->watch(s_token, 1000); });
    // auto render_thread = std::jthread([&s_token] { Render::run(s_token); });
    auto stdin_thread = std::jthread([&s_token] { Input::userInput(s_token); });
    Render::draw();

    // event loop
    while (TUI::p_state->is_running)
    {
        std::unique_lock lock(TUI::mutex_state);
        TUI::cv_state.wait_for(lock, std::chrono::milliseconds(100), [] { return !TUI::p_commands_queue->empty(); });

        if (TUI::p_commands_queue->empty())
        {
            continue;
        }

        TUI::Commands command{TUI::p_commands_queue->front()};

        switch (command)
        {
        case TUI::Commands::ListExercises:
            Render::showExercisesList = !Render::showExercisesList;
            Render::draw();
            break;

        case TUI::Commands::ShowHint:
            Render::showCurrentExerciseHint = !Render::showCurrentExerciseHint;
            Render::draw();
            break;

        case TUI::Commands::Quit:
            Log::info("quitting");
            Log::info("gracefully stop threads");
            stop_src.request_stop();
            Log::info("set is running to false");
            TUI::p_state->is_running = false;
            std::terminate();
            break;

        case TUI::Commands::Clear:
            Log::info("clear terminal");
            Render::clear();
            break;

        case TUI::Commands::Draw:
            Log::info("draw");
            Render::draw();
            break;

        case TUI::Commands::PreviousExercise:
            Log::info("previous exercise");
            if (auto result{exercise_iterator_instance->previous()}; !result)
            {
                Log::warning("{}", result.error());
            }
            lock.unlock();
            exercise_runner_instance->compileCurrentExercise();
            Render::draw();
            lock.lock();
            break;

        case TUI::Commands::NextExercise:
            Log::info("current exercise");
            if (auto result{exercise_iterator_instance->next()}; !result)
            {
                Log::warning("{}", result.error());
            }
            lock.unlock();
            exercise_runner_instance->compileCurrentExercise();
            Render::draw();
            lock.lock();
            break;

        case TUI::Commands::CompileAll:
            Log::info("compile all exercises");
            lock.unlock();
            exercise_runner_instance->compileAllExercises();
            Render::draw();
            lock.lock();
            break;

        case TUI::Commands::Refresh:
            Log::info("refresh");
            lock.unlock();
            exercise_runner_instance->compileCurrentExercise();
            Render::draw();
            lock.lock();
            break;

        case TUI::Commands::Reset:
            Log::info("reset exercise");
            lock.unlock();
            exercise_iterator_instance->reset();
            exercise_runner_instance->compileCurrentExercise();
            Render::draw();
            lock.lock();
            break;
        }

        TUI::p_commands_queue->pop();
    }
}

}; // namespace Runner

void TUI::draw()
{
    Render::draw();
}

void TUI::run(ExerciseIterator::ExerciseDirectories directory)
{
    TUI::p_commands_queue = std::make_unique<std::queue<TUI::Commands>>();
    TUI::p_state = std::make_unique<TUI::State>();

    Runner::run(directory);
}
