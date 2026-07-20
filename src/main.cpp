#include "include/log.hpp"
#include "include/tui.hpp"
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
-p | --patch [-d] -> generate patch files
-s | --solutions [-d] -> run and test cpplings against solutions dir
-d | --debug -> run cpplings in debug mode
)"};

static constexpr void printHelpDocumentation(std::string program_arg, std::string docs)
{
    std::println("{} [OPTION(S)]\n{}", program_arg, docs);
}

}; // namespace Documentation

static constexpr void runCLI([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{

    // NOTE: debug
    Log::configureLogger(true);

    std::vector<std::string> args(argv, argv + argc);

    if (args.size() > 3)
    {
        CLI::Documentation::printHelpDocumentation(args.at(0), CLI::Documentation::MAIN);

        std::println("too many args");

        return;
    }

    if (args.size() == 2 && (args.at(1) == "-d" || args.at(1) == "--debug"))
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
    if (args.at(1) == "-p")
    {
        return;
    }
}

} // namespace CLI

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    CLI::runCLI(argc, argv);
}
