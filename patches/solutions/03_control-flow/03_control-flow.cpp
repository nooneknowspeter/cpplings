// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Loops execute code multiple times while a condition is true.
//
// while loop:
// - Checks condition before each iteration
// - May not execute at all if condition is initially false
//
// do-while loop:
// - Executes at least once
// - Checks condition after each iteration
//
// Key concepts:
// - Loop variable: controls how many times loop runs
// - Update statement: changes loop variable to eventually exit
// - Infinite loops: make sure condition eventually becomes false
//
// https://www.learncpp.com/cpp-tutorial/while-statements/
// https://www.learncpp.com/cpp-tutorial/do-while-statements/

#include <gtest/gtest.h>

// Calculates factorial using a while loop.
// factorial(5) = 5 * 4 * 3 * 2 * 1 = 120
// factorial(0) = 1 (by mathematical definition)
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

// Counts the number of digits in a number using do-while.
// do-while is perfect here because numbers always have at least 1 digit.
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

// Calculates the sum of all integers from 1 to n.
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
