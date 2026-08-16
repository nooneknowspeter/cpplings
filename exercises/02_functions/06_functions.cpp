// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ allows multiple functions to have the same name,
// this technique is called function overloading.
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-function-overloading/
// https://www.learncpp.com/cpp-tutorial/function-overload-differentiation/
// https://www.learncpp.com/cpp-tutorial/function-overload-resolution-and-ambiguous-matches/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement the add function for integers.
// NOTE: Use the test cases as reference.
int add(int x, int y) { return x + y; }

// TODO: Implement the add function for floats.
add(float x, float y) { return x + y; }

// TODO: Implement the add function for doubles.
double add(double x, double y) { return x + y; }

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Functions Functions06") {
  REQUIRE(add(1, 1) == 2);
  REQUIRE(add(5.0f, 5.0f) == 10.0f);
  REQUIRE(add(100.555555, 100.555555) == 201.11111);
}
