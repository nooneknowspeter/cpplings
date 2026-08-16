#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <format>
#include <memory>
#include <queue>

#include "include/exercise_iterator.hpp"
#include "include/tui.hpp"

namespace {

void bootstrap(std::size_t count) {
  TUI::p_state = std::make_unique<TUI::State>();
  TUI::p_commands_queue = std::make_unique<std::queue<TUI::Commands>>();
  for (auto i{0uz}; i < count; ++i) {
    TUI::p_state->list_of_exercises.emplace_back(
        std::format("exercises/0{}/0{}.cpp", i, i));
  }
  TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.front();
}

std::unique_ptr<ExerciseIterator> scan() {
  bootstrap(1);
  auto iterator{ExerciseIterator::getInstance()};
  iterator->scanForExercises(ExerciseIterator::ExerciseDirectories::Exercises);
  return iterator;
}

}  // namespace

TEST_CASE("getExerciseDirectory resolves exercises and solutions",
          "[iterator]") {
  using enum ExerciseIterator::ExerciseDirectories;
  auto exercises{ExerciseIterator::getExerciseDirectory(Exercises)};
  auto solutions{ExerciseIterator::getExerciseDirectory(Solutions)};

  REQUIRE(exercises.has_value());
  REQUIRE(solutions.has_value());

  REQUIRE(exercises.value() == std::filesystem::current_path() / "exercises");
  REQUIRE(solutions.value() == std::filesystem::current_path() / "solutions");
  REQUIRE(std::filesystem::exists(exercises.value()));
  REQUIRE(std::filesystem::exists(solutions.value()));
}

TEST_CASE("scanForExercises collects sorted, filtered exercises",
          "[iterator]") {
  scan();
  auto& list{TUI::p_state->list_of_exercises};

  REQUIRE_FALSE(list.empty());
  REQUIRE(std::is_sorted(list.begin(), list.end()));
  REQUIRE(TUI::p_state->current_exercise == list.front());
  REQUIRE(TUI::p_state->exercises_dir_path ==
          std::filesystem::current_path() / "exercises");

  for (auto const& entry : list) {
    auto ext{entry.extension()};
    REQUIRE_FALSE(ext == std::filesystem::path(".md"));
    REQUIRE_FALSE(ext == std::filesystem::path(".patch"));
    REQUIRE_FALSE(ext == std::filesystem::path(".rej"));
    REQUIRE_FALSE(entry.string().contains("src"));
    REQUIRE_FALSE(entry.string().contains("include"));
  }
}

TEST_CASE("next advances once current exercise compiled", "[iterator]") {
  bootstrap(2);
  auto iterator{ExerciseIterator::getInstance()};
  auto first{TUI::p_state->current_exercise};
  TUI::p_state->did_current_exercise_compile = true;

  iterator->next();

  REQUIRE(TUI::p_state->current_exercise_index.load() == 1);
  REQUIRE(TUI::p_state->current_exercise ==
          TUI::p_state->list_of_exercises.at(1));
  REQUIRE(TUI::p_state->completed_exercises.size() == 1);
  REQUIRE(TUI::p_state->completed_exercises.at(0) == first);
}

TEST_CASE("next is a no-op when current exercise failed to compile",
          "[iterator]") {
  bootstrap(2);
  auto iterator{ExerciseIterator::getInstance()};

  iterator->next();

  REQUIRE(TUI::p_state->current_exercise_index.load() == 0);
  REQUIRE(TUI::p_state->completed_exercises.empty());
}

TEST_CASE("next is a no-op past the last exercise", "[iterator]") {
  bootstrap(2);
  auto iterator{ExerciseIterator::getInstance()};
  auto last_index{TUI::p_state->list_of_exercises.size()};
  TUI::p_state->current_exercise_index = static_cast<std::uint8_t>(last_index);
  TUI::p_state->current_exercise = TUI::p_state->list_of_exercises.back();
  TUI::p_state->did_current_exercise_compile = true;

  iterator->next();

  REQUIRE(TUI::p_state->current_exercise_index.load() == last_index);
  REQUIRE(TUI::p_state->completed_exercises.empty());
}

TEST_CASE("previous moves back and restores completed", "[iterator]") {
  bootstrap(2);
  auto iterator{ExerciseIterator::getInstance()};
  TUI::p_state->did_current_exercise_compile = true;
  iterator->next();

  iterator->previous();

  REQUIRE(TUI::p_state->current_exercise_index.load() == 0);
  REQUIRE(TUI::p_state->current_exercise ==
          TUI::p_state->list_of_exercises.at(0));
  REQUIRE(TUI::p_state->completed_exercises.empty());
}

TEST_CASE("previous is a no-op at the first exercise", "[iterator]") {
  bootstrap(2);
  auto iterator{ExerciseIterator::getInstance()};

  iterator->previous();

  REQUIRE(TUI::p_state->current_exercise_index.load() == 0);
  REQUIRE(TUI::p_state->completed_exercises.empty());
}
