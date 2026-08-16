// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Loops execute code multiple times while a condition is true.
//
// while loop:
// - Checks condition before each iteration
// - May not execute at all if condition is initially false
//
// do-while loop:
// - Executes at least once
// - Checks condition after each iteration
//
// Key concepts:
// - Loop variable: controls how many times loop runs
// - Update statement: changes loop variable to eventually exit
// - Infinite loops: make sure condition eventually becomes false
//
// https://www.learncpp.com/cpp-tutorial/while-statements/
// https://www.learncpp.com/cpp-tutorial/do-while-statements/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Calculates factorial using a while loop.
// factorial(5) = 5 * 4 * 3 * 2 * 1 = 120
// factorial(0) = 1 (by mathematical definition)
int factorial(int n) {
  int result = 1;
  while (n > 0) {
    result *= n;
    n--;
  }
  return result;
}

// Counts the number of digits in a number using do-while.
// do-while is perfect here because numbers always have at least 1 digit.
int countDigits(int n) {
  int count = 0;
  do {
    count++;
    n /= 10;
  } while (n > 0);
  return count;
}

// Calculates the sum of all integers from 1 to n.
int sumToN(int n) {
  int sum = 0;
  int i = 1;
  while (i <= n) {
    sum += i;
    i++;
  }
  return sum;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("ControlFlow ControlFlow03") {
  REQUIRE(factorial(5) == 120);
  REQUIRE(factorial(0) == 1);
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(10) == 3628800);

  REQUIRE(countDigits(12345) == 5);
  REQUIRE(countDigits(0) == 1);
  REQUIRE(countDigits(9) == 1);

  REQUIRE(sumToN(5) == 15);
  REQUIRE(sumToN(10) == 55);
  REQUIRE(sumToN(1) == 1);
}
