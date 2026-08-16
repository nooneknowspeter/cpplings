// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Fold expressions (C++17) simplify variadic template operations.
//
// Without fold expressions, you needed recursion:
// template<typename T>
// T sum(T v) { return v; }
// template<typename T, typename... Args>
// T sum(T first, Args... args) { return first + sum(args...); }
//
// With fold expressions, it's one line:
// template<typename... Args>
// auto sum(Args... args) { return (... + args); }
//
// Four fold forms:
// 1. Unary left fold:  (... op pack)    -> ((pack1 op pack2) op ...) op packN
// 2. Unary right fold:  (pack op ...)    -> pack1 op (pack2 op (... op packN))
// 3. Binary left fold:  (init op ... op pack)
// 4. Binary right fold:  (pack op ... op init)
//
// When to use each:
// - Left folds are usually what you want for commutative ops (+, *, &&, ||)
// - Right folds can be needed for non-commutative ops
// - Binary folds add an initial value
//
// https://www.learncpp.com/cpp-tutorial/fold-expressions/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

// Sums all arguments using unary left fold.
template <typename... Args>
auto sum(Args... args) {
  return (... + args);
}

// Prints all arguments using comma fold.
// The comma operator evaluates left, then right, returns right.
template <typename T, typename... Args>
void printAll(T first, Args... args) {
  std::cout << first << " ";
  (..., (std::cout << args << " "));
}

// Returns true if ALL arguments are positive (logical AND fold).
template <typename... Args>
bool allPositive(Args... args) {
  return (... && (args > 0));
}

// Returns true if ANY argument is true (logical OR fold).
template <typename... Args>
bool anyTrue(Args... args) {
  return (... || args);
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Generics Generics03") {
  REQUIRE(sum(1, 2, 3, 4, 5) == 15);
  REQUIRE(sum(1) == 1);
  REQUIRE(sum(1.5, 2.5, 3.0) == 7.0);

  REQUIRE(allPositive(1, 2, 3));
  REQUIRE_FALSE(allPositive(1, -2, 3));
  REQUIRE(allPositive(5));

  REQUIRE(anyTrue(false, true, false));
  REQUIRE_FALSE(anyTrue(false, false, false));
  REQUIRE(anyTrue(true));
}
