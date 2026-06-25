// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Control flow statements allow you to execute code conditionally
// or repeatedly. The most basic is the if statement.
//
// if statements evaluate a condition (must be boolean).
// If true, the block executes. Otherwise, it checks else if or else.
//
// Key concepts:
// - Conditions are always boolean (true/false)
// - else if chains are evaluated top to bottom
// - The first true condition wins
// - else is executed if no condition is true
//
// Best practices:
// - Always use braces {} for clarity
// - Handle edge cases (like the else)
// - Keep conditions simple and readable
//
// https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/
// https://www.learncpp.com/cpp-tutorial/if-else-statements/

#include <gtest/gtest.h>

// Returns a grade letter based on the numeric score.
char getGrade(int score)
{
    if (score >= 90)
    {
        return 'A';
    }
    else if (score >= 80)
    {
        return 'B';
    }
    else if (score >= 70)
    {
        return 'C';
    }
    else if (score >= 60)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

// Checks if a number is positive, negative, or zero.
// Returns 1 for positive, -1 for negative, 0 for zero.
int checkNumber(int num)
{
    if (num > 0)
    {
        return 1;
    }
    else if (num < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ControlFlow, ControlFlow01)
{
    ASSERT_EQ(getGrade(95), 'A');
    ASSERT_EQ(getGrade(85), 'B');
    ASSERT_EQ(getGrade(75), 'C');
    ASSERT_EQ(getGrade(65), 'D');
    ASSERT_EQ(getGrade(55), 'F');

    ASSERT_EQ(checkNumber(5), 1);
    ASSERT_EQ(checkNumber(-3), -1);
    ASSERT_EQ(checkNumber(0), 0);
}
