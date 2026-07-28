#include "include/tui.hpp"
#include "include/ascii.hpp"
#include "include/exercise_iterator.hpp"
#include "include/file_watcher.hpp"
#include <chrono>
#include <exception>
#include <include/log.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <print>
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
    {"n", TUI::Commands::NextExercise}, {"p", TUI::Commands::PreviousExercise},
    {"c", TUI::Commands::CompileAll},   {"r", TUI::Commands::Refresh},
    {"x", TUI::Commands::Reset},        {"q", TUI::Commands::Quit},
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

    static void run(std::stop_token s_token)
    {
        auto current_tick{TUI::p_state->render_tick.load()};

        while (!s_token.stop_requested())
        {
            if (current_tick != TUI::p_state->render_tick.load())
            {
                current_tick = TUI::p_state->render_tick.load();
                draw();
            }
        }
    }

    static void draw()
    {
        Log::info("clearing terminal");
        Render::clear();

        Log::info("tick: {}", ++tick);
        Log::info("drawing");
        std::println("{}", ASCII::ART);

        // TODO: print exercise runner output

        Render::compileStatus();

        Render::progress();

        Render::help();
    }

    static void clear()
    {
        std::print("{}", ASCII::Cursor::CLEAR_TERMINAL);
    }

  private:
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
        const auto REMAINING_EXERCISES{TUI::p_state->list_of_exercises.size() -
                                       TUI::p_state->completed_exercises.size()};

        // std::println("Progress: [{}] {}/{}", ascii_bar, TUI::p_state->current_exercise_index,
        // TUI::p_state->list_of_exercises.size());

        // "Progress: [{}] {}/{}" -> "Progress: [######>-----] 6/7"
        std::print("Progress: [");

        for (auto completed_exercise : TUI::p_state->completed_exercises)
        {
            std::print("#");
        }

        std::print(">");

        for (auto i{REMAINING_EXERCISES}; i > 0; --i)
        {
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
        std::print("{}h{}: hint / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}l{}: list / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}q{}: quit ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("-> ");
    }
};

auto Render::tick{0uz};

namespace Runner
{

void run(ExerciseIterator::ExerciseDirectories directory)
{
    // bootstrap
    auto exercise_iterator_instance{ExerciseIterator::getInstance()};
    exercise_iterator_instance->updateExerciseDirectory(directory);
    exercise_iterator_instance->scanForExercises();

    TUI::p_commands_queue->push(TUI::Commands::CompileAll);

    std::stop_source stop_src;
    std::stop_token s_token{stop_src.get_token()};
    auto file_watcher_thread = std::jthread([&s_token] { FileWatcher::getInstance()->watch(s_token, 1000); });
    auto render_thread = std::jthread([&s_token] { Render::run(s_token); });
    auto stdin_thread = std::jthread([&s_token] { Input::userInput(s_token); });

    // event loop
    while (!s_token.stop_requested())
    {
        std::unique_lock lock(TUI::mutex_state);
        TUI::cv_state.wait_for(lock, std::chrono::milliseconds(100), [] { return !TUI::p_commands_queue->empty(); });

        if (TUI::p_commands_queue->empty())
        {
            // TUI::p_state->render_tick++;
            continue;
        }

        TUI::Commands command{TUI::p_commands_queue->front()};

        switch (command)
        {
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
            TUI::p_state->render_tick++;
            break;

        case TUI::Commands::PreviousExercise:
            Log::info("previous exercise");
            exercise_iterator_instance->previous();
            TUI::p_state->render_tick++;
            break;

        case TUI::Commands::NextExercise:
            Log::info("current exercise");
            exercise_iterator_instance->next();
            TUI::p_state->render_tick++;
            break;

        case TUI::Commands::CompileAll:
            Log::info("compile all exercises");
            TUI::p_state->render_tick++;
            break;

        case TUI::Commands::Refresh:
            Log::info("refresh");
            TUI::p_state->render_tick++;
            break;

        case TUI::Commands::Reset:
            Log::info("reset exercise");
            exercise_iterator_instance->reset();
            TUI::p_state->render_tick++;
            break;
        }

        TUI::p_commands_queue->pop();
    }
}

}; // namespace Runner

void TUI::run(ExerciseIterator::ExerciseDirectories directory)
{
    TUI::p_commands_queue = std::make_unique<std::queue<TUI::Commands>>();
    TUI::p_state = std::make_unique<TUI::State>();

    Runner::run(directory);
}
