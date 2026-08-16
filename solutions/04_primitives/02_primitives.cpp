// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Boolean type (bool):
// - Represents true/false values
// - Size: typically 1 byte
// - Values: true (1) or false (0)
// - When printed, shows 1 or 0
//
// Character type (char):
// - Represents a single character
// - Size: typically 1 byte (8 bits)
// - Stored as ASCII value
// - Use single quotes for char literals: 'A', '5', '\n'
//
// Key concepts:
// - bool can be used in conditions directly
// - char comparisons use ASCII values
// - std::tolower/toupper for case-insensitive comparison
//
// https://www.learncpp.com/cpp-tutorial/boolean-values/
// https://www.learncpp.com/cpp-tutorial/char/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <string>

// Checks if a character is a vowel (case-insensitive).
bool isVowel(char c) {
  c = std::tolower(c);
  return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

// Checks if a character is a digit ('0' through '9').
bool isDigit(char c) { return c >= '0' && c <= '9'; }

// Converts a boolean to a string.
const char* boolToString(bool b) { return b ? "true" : "false"; }

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Primitives Primitives02") {
  REQUIRE(isVowel('a'));
  REQUIRE(isVowel('E'));
  REQUIRE_FALSE(isVowel('b'));
  REQUIRE_FALSE(isVowel('Z'));

  REQUIRE(isDigit('5'));
  REQUIRE(isDigit('0'));
  REQUIRE_FALSE(isDigit('a'));
  REQUIRE_FALSE(isDigit('#'));

  REQUIRE(std::string(boolToString(true)) == "true");
  REQUIRE(std::string(boolToString(false)) == "false");
}
