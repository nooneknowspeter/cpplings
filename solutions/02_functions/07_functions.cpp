// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Recursion is when a function calls itself to solve a problem
// by breaking it down into smaller, similar subproblems.
//
// The fibonacci sequence is a classic example of recursion:
// f(0) = 0
// f(1) = 1
// f(n) = f(n-1) + f(n-2) for n > 1
//
// Every recursive function needs:
// 1. A base case (when to stop) - here: n == 0 or n == 1
// 2. A recursive case (call itself with modified argument) - here: f(n-1) + f(n-2)
//
// Warning: This naive fibonacci implementation is inefficient for large n
// because it recalculates the same values many times.
// In practice, consider using iteration or memoization.
//
// https://www.learncpp.com/cpp-tutorial/recursion/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

// The recursive fibonacci function.
// Base case: when n is 0 or 1, return n itself.
// Recursive case: return the sum of the previous two fibonacci numbers.
int fibonacci(int n)
{
    // Base case: fibonacci(0) = 0, fibonacci(1) = 1
    if (n == 0 || n == 1)
    {
        return n;
    }

    // Recursive case: f(n) = f(n-1) + f(n-2)
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions07")
{
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    for (int n = 0; n <= 10; ++n)
    {
        if (n == 10)
        {
            std::cout << fibonacci(n) << ",..." << std::endl;
            continue;
        }

        std::cout << fibonacci(n) << ", ";
    }

    std::cout.rdbuf(old);
    std::string output{buffer.str()};

    REQUIRE(output == "0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,...\n");

    std::cout << "\n" << output << "\n";
}
