// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Function templates allow writing generic functions that work with any type.
//
// Syntax:
// template <typename T>
// return_type functionName(T param) { ... }
//
// The compiler generates specific code for each type used.
// This is called template instantiation.
//
// https://www.learncpp.com/cpp-tutorial/function-templates/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a template function that returns the maximum of two values.
// Use if-else or the ternary operator.
template <typename T> maximum(T a, T b)
{
    return (a > b) ? a : b;
}

// TODO: Implement a template function that swaps two values.
template <typename T> void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// TODO: Implement a template function that returns the absolute value.
template <typename T> T absolute(T value)
{
    if (value < T{})
    {
        return -value;
    }
    return value;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Generics Generics01")
{
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
