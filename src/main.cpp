#include <algorithm>
#include <jal.hpp>
#include <memory>
#include <print>
#include <string>
#include <vector>

#include "./include/ascii.hpp"
#include "./include/exercise_iterator.hpp"
#include "./include/patch_system.hpp"
#include "./include/tui.hpp"

using namespace jal;

namespace CLI {

namespace Documentation {

static const std::string MAIN{R"(
options:
-h -> output this help documentation
-d -> run cpplings in debug mode
-p [-d] -> generate patch files
-s [-d] -> run and test cpplings against solutions dir
)"};

static constexpr void printHelpDocumentation(std::string program_arg,
                                             std::string docs) {
  std::println("{}\n{} [OPTION(S)]\n{}", ASCII::ART, program_arg, docs);
}

};  // namespace Documentation

static constexpr void runCLI([[maybe_unused]] int argc,
                             [[maybe_unused]] char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() > 3) {
    CLI::Documentation::printHelpDocumentation(args.at(0),
                                               CLI::Documentation::MAIN);

    std::println("too many args");

    return;
  }

  if (std::ranges::find(args, "-h") != args.end()) {
    CLI::Documentation::printHelpDocumentation(args.at(0),
                                               CLI::Documentation::MAIN);

    return;
  }

  if (std::ranges::find(args, "-d") != args.end()) {
    Log::configureLogger(true);
  }

  if (std::ranges::find(args, "-p") != args.end()) {
    Log::info("run patch system");

    TUI::p_state = std::make_unique<TUI::State>();
    PatchSystem::generatePatches();

    return;
  }

  if (std::ranges::find(args, "-s") != args.end()) {
    Log::info("run tui against solutions");
    TUI::run(ExerciseIterator::ExerciseDirectories::Solutions);

    return;
  }

  Log::info("running tui");
  TUI::run();
}

}  // namespace CLI

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
  CLI::runCLI(argc, argv);
}
