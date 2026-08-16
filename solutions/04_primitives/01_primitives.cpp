// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ has several fundamental data types for storing different kinds of values.
//
// Integer types (whole numbers):
// - int: typically 32 bits, range: -2,147,483,648 to 2,147,483,647
// - short: typically 16 bits
// - long: at least 32 bits
// - long long: at least 64 bits
//
// Floating-point types (decimal numbers):
// - float: typically 32 bits, ~6-7 significant digits
// - double: typically 64 bits, ~15 significant digits
// - long double: even more precision
//
// The 'auto' keyword deduces the type from the initializer.
// It's useful when the type is verbose or obvious.
//
// Best practices:
// - Use 'int' for most integer values
// - Use 'double' for most floating-point calculations
// - Use 'const' for values that shouldn't change
// - Use 'auto' when the type is obvious from context
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-fundamental-data-types/
// https://www.learncpp.com/cpp-tutorial/floating-point-numbers/

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

// Integer variable storing age.
int age = 25;

// Double variable storing a price.
double price = 19.99;

// Constant integer for maximum size.
const int MAX_SIZE = 100;

// Auto-deduced integer (compiler sees 42 and deduces int).
auto quantity = 42;

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Primitives Primitives01") {
  REQUIRE(age == 25);
  REQUIRE(price == Catch::Approx(19.99));
  REQUIRE(MAX_SIZE == 100);
  REQUIRE(quantity == 42);
}
