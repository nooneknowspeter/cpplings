// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Type deduction with auto:
// - auto deduces type from the initializer
// - const, volatile, and references are dropped by default
// - Use const auto& to preserve constness
// - Use auto& to deduce a reference type
//
// constexpr (C++11):
// - Evaluated at compile time
// - Enables optimizations
// - Required for array sizes, template arguments
// - constexpr functions must be simple (C++14 relaxed this)
//
// constexpr vs const:
// - const: run-time constant, value doesn't change after initialization
// - constexpr: compile-time constant, known at compile time
//
// https://www.learncpp.com/cpp-tutorial/type-deduction-with-auto/
// https://www.learncpp.com/cpp-tutorial/constexpr-variables/

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Adds values and returns the result.
// Type is deduced as double (the wider type).
auto addWithAuto(int a, double b)
{
    auto result = a + b;
    return result;
}

// Calculates the square of an integer at compile time.
constexpr int square(int x)
{
    return x * x;
}

// Compile-time constant (computed during compilation).
constexpr int SQUARED_TEN = square(10);

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("TypeCoercion TypeCoercion03")
{
    REQUIRE(addWithAuto(3, 4.5) == Catch::Approx(7.5));

    REQUIRE(square(5) == 25);
    REQUIRE(square(-3) == 9);
    REQUIRE(square(0) == 0);

    REQUIRE(SQUARED_TEN == 100);
}
