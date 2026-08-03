// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Type deduction with auto:
// - auto deduces the type from the initializer
// - References are dropped unless & is used
// - const and volatile are dropped unless explicitly kept
//
// constexpr: compile-time constant evaluation
// - Value is computed at compile time
// - Can be used in contexts requiring constants
//
// https://www.learncpp.com/cpp-tutorial/type-deduction-with-auto/
// https://www.learncpp.com/cpp-tutorial/constexpr-variables/

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Use auto to deduce the type of a value.
// Return the result of adding an int and a double.
addWithAuto(int a, double b)
{
    auto result = a + b;
    return result;
}

// TODO: Create a constexpr function that calculates the square of a number.
// This should be evaluable at compile time.
constexpr int square(int x)
{
    return x * x;
}

// TODO: Create a constexpr variable that stores the result of square(10).
// This will be computed at compile time.
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
