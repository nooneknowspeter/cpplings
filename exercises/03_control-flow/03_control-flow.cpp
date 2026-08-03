// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Loops allow you to execute code multiple times.
//
// while loop:
// while (condition) {
//     // code
// }
//
// do-while loop (always executes at least once):
// do {
//     // code
// } while (condition);
//
// https://www.learncpp.com/cpp-tutorial/while-statements/
// https://www.learncpp.com/cpp-tutorial/do-while-statements/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a function that calculates the factorial of a number
// using a while loop.
// factorial(5) = 5 * 4 * 3 * 2 * 1 = 120
// factorial(0) = 1 (by definition)
int factorial(int n)
{
    int result = 1;
    while (n = 0)
    {
        result *= n;
        n--;
    }
    return result;
}

// TODO: Implement a function that counts the digits in a number
// using a do-while loop.
// countDigits(12345) = 5
// countDigits(0) = 1
int countDigits(int n)
{
    int count = 0;
    do
    {
        count++;
        n /= 10;
    } while (n > 0);
    return count;
}

// TODO: Implement a function that calculates the sum of all numbers
// from 1 to n using a while loop.
int sumToN(int n)
{
    int sum = 0;
    int i = 1;
    while (i <= n)
    {
        sum += i;
        i++;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("ControlFlow ControlFlow03")
{
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
