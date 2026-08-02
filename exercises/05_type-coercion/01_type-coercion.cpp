// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Type coercion (type conversion) is converting one type to another.
//
// Implicit conversion (coercion):
// - Done automatically by the compiler
// - Usually widening (small type -> large type)
// - int to double, char to int, etc.
//
// Explicit conversion (casting):
// - Done manually by the programmer
// - static_cast<T>(value) - compile-time checked
// - reinterpret_cast<T>(value) - reinterpret bits
// - const_cast<T>(value) - remove/add const
//
// https://www.learncpp.com/cpp-tutorial/implicit-type-conversion-coercion/
// https://www.learncpp.com/cpp-tutorial/explicit-type-conversion-casting/

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a function that adds an int and a double.
// Return the result as a double (implicit conversion of int to double).
double addIntDouble(int a, double b)
{
    return a + b;
}

// TODO: Implement a function that divides two integers and returns a double.
// Use explicit casting: static_cast<double>(value)
double divideInts(int a, int b)
{
    return static_cast<double>(a) / static_cast<double>(b);
}

// TODO: Implement a function that truncates a double to an int.
// Use static_cast to explicitly convert.
int truncateToInt(double d)
{
    return static_cast<int>(d);
}

// TODO: Implement a function that rounds a double to the nearest int.
// Hint: Add 0.5 before truncating for positive numbers.
int roundToInt(double d)
{
    if (d >= 0)
    {
        return static_cast<int>(d + 0.5);
    }
    else
    {
        return static_cast<int>(d - 0.5);
    }
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("TypeCoercion TypeCoercion01")
{
    REQUIRE(addIntDouble(5, 3.5) == Catch::Approx(8.5));
    REQUIRE(addIntDouble(10, 2.0) == Catch::Approx(12.0));

    REQUIRE(divideInts(10, 4) == Catch::Approx(2.5));
    REQUIRE(divideInts(7, 2) == Catch::Approx(3.5));

    REQUIRE(truncateToInt(3.7) == 3);
    REQUIRE(truncateToInt(3.2) == 3);

    REQUIRE(roundToInt(3.7) == 4);
    REQUIRE(roundToInt(3.2) == 3);
    REQUIRE(roundToInt(-3.7) == -4);
}
