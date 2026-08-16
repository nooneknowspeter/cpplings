// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Numeric promotions and conversions:
// - bool -> int
// - char -> int
// - short -> int
// - float -> double
//
// When mixing types in expressions, the compiler promotes smaller
// types to larger ones (arithmetic conversions).
//
// Narrowing conversion: converting a larger type to a smaller one
// Can lose data - use with caution!
//
// https://www.learncpp.com/cpp-tutorial/arithmetic-conversions/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a function that safely converts a char to an int.
// This is a widening conversion (char -> int), no data loss.
int charToInt(char c) { return c; }

// TODO: Implement a function that converts an int to a char.
// This is a narrowing conversion - use static_cast to make it explicit.
intToChar(int i) { return static_cast<char>(i); }

// TODO: Implement a function that checks if a double can be safely
// converted to an int without losing fractional part.
bool isWholeNumber(double d) {
  int i = static_cast<int>(d);
  return static_cast<double>(i) == d;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("TypeCoercion TypeCoercion02") {
  REQUIRE(charToInt('A') == 65);
  REQUIRE(charToInt('0') == 48);

  REQUIRE(intToChar(65) == 'A');
  REQUIRE(intToChar(97) == 'a');

  REQUIRE(isWholeNumber(5.0));
  REQUIRE(isWholeNumber(-3.0));
  REQUIRE_FALSE(isWholeNumber(5.5));
  REQUIRE_FALSE(isWholeNumber(-3.7));
}
