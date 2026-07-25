#include "include/exercise_iterator.hpp"
#include "include/log.hpp"
#include "include/tui.hpp"
#include <algorithm>
#include <print>
#include <string>
#include <vector>

namespace CLI
{

namespace Documentation
{

static const std::string MAIN{R"(
options:
-h | --help | help -> output this help documentation
-d -> run cpplings in debug mode
-p [-d] -> generate patch files
-s [-d] -> run and test cpplings against solutions dir
-c < zig | clang | gcc> [-d] -> run cpplings with a preferred compiler
)"};

static constexpr void printHelpDocumentation(std::string program_arg, std::string docs)
{
    std::println("{} [OPTION(S)]\n{}", program_arg, docs);
}

}; // namespace Documentation

static constexpr void runCLI([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    Log::configureLogger(true);

    if (args.size() > 3)
    {
        CLI::Documentation::printHelpDocumentation(args.at(0), CLI::Documentation::MAIN);

        std::println("too many args");

        return;
    }

    if (std::find(args.begin(), args.end(), "-h") != args.end())
    {
        CLI::Documentation::printHelpDocumentation(args.at(0), CLI::Documentation::MAIN);

        return;
    }

    if (std::find(args.begin(), args.end(), "-d") != args.end())
    {
        Log::configureLogger(true);
    }

    if (args.size() == 1)
    {
        Log::info("running tui");
        TUI::run();

        return;
    }

    // TODO: run patch system
    if (std::find(args.begin(), args.end(), "-p") != args.end())
    {
        Log::info("run patch system");

        return;
    }

    if (std::find(args.begin(), args.end(), "-s") != args.end())
    {
        Log::info("run tui against solutions");
        TUI::run(ExerciseIterator::ExerciseDirectories::Solutions);

        return;
    }
}

} // namespace CLI

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    CLI::runCLI(argc, argv);
}
