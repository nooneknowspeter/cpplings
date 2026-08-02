// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Evey programmer knows that functions are resuable pieces of code.
//
// Each function is called and assigned/"pushed" on to the stack (fragment of memory).
// Read more about the stack here: https://en.wikipedia.org/wiki/Stack-based_memory_allocation/
//
// A C++ function includes:
//  - The functions signature: int f(int x)
//                              |  | |
//                              |  | parameter list (parameter datatype parameter name, ...)
//                              |  |
//                              |  function name
//                              |
//                              function return type
//
//  - Function clause/body:
//			{ -> function scope beginning
//					return x; -> return (depends on function return type; will be explained in an upcoming exercise)
//  		} -> function scope ending
//
// Functions are normally named using the camelCase convention.
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-functions/
// https://www.learncpp.com/cpp-tutorial/function-return-values-value-returning-functions/
// https://www.learncpp.com/cpp-tutorial/why-functions-are-useful-and-how-to-use-them-effectively/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

// TODO: Create the function signature.
// Add the function return type, name "f" and a parameter "x" with an int datatype.
int f(int x)
{
    int y{x + 1};

    std::cout << "x: " << x << " " << "y: " << y << std::endl;

    return y;
}

int main(int argc, char **argv)
{
    // TODO: Call the function and pass in an appropriate parameter.
    f(10);

    std::cout << "\n\n" << "Testing output begins here\n--------------------------" << "\n";
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Function01")
{
    REQUIRE(f(0) == 1);
    REQUIRE(f(1) == 2);
    REQUIRE(f(2) == 3);
};
