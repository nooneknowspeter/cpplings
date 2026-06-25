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

#include <gtest/gtest.h>

// TODO: Implement a function that calculates the factorial of a number
// using a while loop.
// factorial(5) = 5 * 4 * 3 * 2 * 1 = 120
// factorial(0) = 1 (by definition)
int factorial(int n)
{
    int result = 1;
    while (n > 0)
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ControlFlow, ControlFlow03)
{
    ASSERT_EQ(factorial(5), 120);
    ASSERT_EQ(factorial(0), 1);
    ASSERT_EQ(factorial(1), 1);
    ASSERT_EQ(factorial(10), 3628800);

    ASSERT_EQ(countDigits(12345), 5);
    ASSERT_EQ(countDigits(0), 1);
    ASSERT_EQ(countDigits(9), 1);

    ASSERT_EQ(sumToN(5), 15);
    ASSERT_EQ(sumToN(10), 55);
    ASSERT_EQ(sumToN(1), 1);
}
