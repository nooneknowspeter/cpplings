#include "include/tui.hpp"
#include "include/ascii.hpp"
#include <exception>
#include <include/log.hpp>
#include <iostream>
#include <memory>
#include <print>
#include <unordered_map>
#include <utility>

namespace TUI
{
std::unique_ptr<std::deque<TUI::Commands>> p_commands_queue;

std::unique_ptr<TUI::State> p_state;
} // namespace TUI

namespace Input
{

std::unordered_map<std::string, TUI::Commands> command_map{
    {"n", TUI::Commands::NextExercise}, {"p", TUI::Commands::PreviousExercise},
    {"c", TUI::Commands::CompileAll},   {"r", TUI::Commands::Refresh},
    {"x", TUI::Commands::Reset},        {"q", TUI::Commands::Quit},
};

static void userInput()
{

    std::string input;

    std::getline(std::cin, input);

    switch (Input::command_map[input])
    {
    case TUI::Commands::NextExercise:
        Log::info("next exercise");
        TUI::p_commands_queue->push_back(TUI::Commands::NextExercise);

    case TUI::Commands::PreviousExercise:
        Log::info("previous exercise");
        TUI::p_commands_queue->push_back(TUI::Commands::PreviousExercise);

    case TUI::Commands::CompileAll:
        Log::info("compile all exercises");
        TUI::p_commands_queue->push_back(TUI::Commands::CompileAll);

    case TUI::Commands::Refresh:
        Log::info("compile current exercise");
        TUI::p_commands_queue->push_back(TUI::Commands::Refresh);

    case TUI::Commands::Reset:
        Log::info("reset exercise to previous state");
        TUI::p_commands_queue->push_back(TUI::Commands::Reset);

    case TUI::Commands::Quit:
        Log::info("quit");
        TUI::p_commands_queue->push_back(TUI::Commands::Quit);
    }
}

}; // namespace Input

struct Render
{

    static size_t tick;

    static void draw()
    {
        Log::info("clearing terminal");
        Render::clear();

        Log::info("tick: {}", ++tick);
        Log::info("drawing");
        std::println("{}", ASCII::ART);

        std::print("{}n{}: next / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}p{}: previous / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}c{}: check all / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}r{}: refresh / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}x{}: reset / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}h{}: hint / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}l{}: list / ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("{}q{}: quit ", ASCII::Styles::BOLD, ASCII::Styles::CLEAR_STYLE);
        std::print("-> ");

        Input::userInput();
    }

    static void clear()
    {
        std::print("{}", ASCII::Cursor::CLEAR_TERMINAL);
    }

  private:
    void progressBar()

    {
        Input::userInput();
    }
};

auto Render::tick{0uz};

namespace Runner
{

void run()
{
    while (true)
    {
        if (!TUI::p_commands_queue->empty())
        {
            [[maybe_unused]] TUI::Commands command{TUI::p_commands_queue->front()};

            switch (command)
            {

            case TUI::Commands::Quit:
                Log::info("quitting");
                Log::warning("calling std::terminate");
                std::terminate();

            case TUI::Commands::NextExercise:
                // TODO:

            case TUI::Commands::PreviousExercise:
                // TODO:

            case TUI::Commands::CompileAll:
                // TODO:

            case TUI::Commands::Refresh:
                // TODO:

            case TUI::Commands::Reset:
                // TODO:
            }

            TUI::p_commands_queue->pop_front();
        }

        Render::draw();
    }
}

}; // namespace Runner

// TODO:
void TUI::run()
{

    TUI::p_commands_queue = std::make_unique<std::deque<TUI::Commands>>();

    TUI::p_state = std::make_unique<TUI::State>();

    Runner::run();
}
