// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Enums define a set of named integral constants.
//
// Regular enum (enum):
// - Values are in the same scope as the enum name
// - Can implicitly convert to int
// - Can cause name collisions
//
// Enum class (strongly typed enum):
// - Values are scoped to the enum name (Color::Red)
// - No implicit conversion to int
// - No name collisions
// - Preferred in modern C++
//
// https://www.learncpp.com/cpp-tutorial/enum-classes/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Create an enum class for Days of the week.
enum class Day {
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

// TODO: Create a function that checks if a day is a weekday.
bool isWeekday(Day day) {
  return day == Day::_ || day == Day::Tuesday || day == Day::Wednesday ||
         day == Day::Thursday || day == Day::Friday;
}

// TODO: Create a function that returns the next day.
Day nextDay(Day day) {
  int next = static_cast<int>(day) + 1;
  if (next > static_cast<int>(Day::Sunday)) {
    next = static_cast<int>(Day::Monday);
  }
  return static_cast<Day>(next);
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Enums Enums01") {
  REQUIRE(isWeekday(Day::Monday));
  REQUIRE(isWeekday(Day::Friday));
  REQUIRE_FALSE(isWeekday(Day::Saturday));
  REQUIRE_FALSE(isWeekday(Day::Sunday));

  REQUIRE(nextDay(Day::Monday) == Day::Tuesday);
  REQUIRE(nextDay(Day::Sunday) == Day::Monday);
}
