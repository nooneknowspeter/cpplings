// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Header files organize code and enable code reuse.
//
// #include copies the header content into your file.
// Headers typically contain:
// - Declarations (function prototypes, class definitions)
// - Constants
// - Template definitions
//
// Best practices:
// - Use #ifndef/#define/#endif guards to prevent multiple inclusion
// - Separate declarations (.h) from implementations (.cpp)
// - Include only what's needed
//
// https://www.learncpp.com/cpp-tutorial/header-files/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Create a function declaration for add.
// The implementation is in src/math_utils.cpp
int add(int a, int b);

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Headers Headers01") {
  REQUIRE(add(2, 3) == 5);
  REQUIRE(add(-1, 1) == 0);
  REQUIRE(add(0, 0) == 0);
}
