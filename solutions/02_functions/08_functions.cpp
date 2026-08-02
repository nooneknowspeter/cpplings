// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// A lambda expression (also known as a lambda or anonymous function)
// is a shorthand way to define a function without giving it a name.
//
// Lambda syntax: [capture clause](parameters) -> return_type { body }
//
// - Capture clause []: specifies which variables from the enclosing scope to capture
// - Parameters (): like regular function parameters
// - Return type ->: can be omitted for type inference
// - Body {}: the actual code
//
// Common capture modes:
// [] - capture nothing
// [=] - capture everything by value (copy)
// [&] - capture everything by reference
// [x, &y] - capture x by value, y by reference
//
// Lambdas are useful for:
// - Short-lived functions (callbacks, predicates)
// - Inline function objects
// - STL algorithms
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/
// https://www.learncpp.com/cpp-tutorial/lambda-captures/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

// Lambda that checks if a number is even.
// [] means no captures, (int n) is the parameter, -> bool is explicit return.
auto isEven = [](int n) -> bool { return n % 2 == 0; };

// Lambda that doubles a number.
// Return type is inferred as int.
auto doubleIt = [](int n) { return n * 2; };

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions08")
{
    REQUIRE(isEven(2));
    REQUIRE(isEven(4));
    REQUIRE_FALSE(isEven(3));
    REQUIRE_FALSE(isEven(5));

    REQUIRE(doubleIt(5) == 10);
    REQUIRE(doubleIt(0) == 0);
    REQUIRE(doubleIt(-3) == -6);
}

TEST_CASE("Functions Functions08_Capture")
{
    // Lambda with capture by value [=].
    const int multiplier = 3;
    auto multiplyByGlobal = [=](int n) { return n * multiplier; };
    REQUIRE(multiplyByGlobal(4) == 12);
}
