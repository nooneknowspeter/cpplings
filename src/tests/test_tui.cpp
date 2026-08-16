#include <catch2/catch_test_macros.hpp>

#include "../include/tui.hpp"

TEST_CASE("State has sane defaults", "[tui]") {
  TUI::State state;
  REQUIRE(state.is_running.load());
  REQUIRE(state.new_user.load());
  REQUIRE(state.current_exercise_index.load() == 0);
  REQUIRE_FALSE(state.is_current_exercise_compiling.load());
  REQUIRE_FALSE(state.did_current_exercise_compile.load());
  REQUIRE(state.list_of_exercises.empty());
  REQUIRE(state.completed_exercises.empty());
  REQUIRE(state.current_exercise.empty());
}
