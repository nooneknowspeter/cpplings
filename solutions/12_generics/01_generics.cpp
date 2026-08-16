// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Function templates allow writing code that works with any data type.
//
// Key concepts:
// - template <typename T>: declares a type parameter
// - T is a placeholder that gets replaced with actual types
// - The compiler generates specialized code for each type used
// - This is called template instantiation
//
// Benefits of templates:
// - Code reusability
// - Type safety (with proper constraints)
// - Performance (no runtime overhead)
//
// When to use templates:
// - When the algorithm is the same for different types
// - When you need type-safe containers
// - When you want to avoid code duplication
//
// https://www.learncpp.com/cpp-tutorial/function-templates/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Returns the maximum of two values.
template <typename T>
T maximum(T a, T b) {
  return (a > b) ? a : b;
}

// Swaps two values.
template <typename T>
void swap(T& a, T& b) {
  T temp = a;
  a = b;
  b = temp;
}

// Returns the absolute value of a number.
template <typename T>
T absolute(T value) {
  if (value < T{}) {
    return -value;
  }
  return value;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Generics Generics01") {
  REQUIRE(maximum(5, 10) == 10);
  REQUIRE(maximum(3.14, 2.71) == 3.14);
  REQUIRE(maximum('a', 'z') == 'z');

  int x = 5, y = 10;
  swap(x, y);
  REQUIRE(x == 10);
  REQUIRE(y == 5);

  REQUIRE(absolute(-5) == 5);
  REQUIRE(absolute(5) == 5);
  REQUIRE(absolute(-3.14) == 3.14);
}
