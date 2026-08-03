// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Control flow statements allow you to execute code conditionally
// or repeatedly. The most basic is the if statement.
//
// Syntax:
// if (condition) {
//     // code to execute if condition is true
// }
//
// The condition must evaluate to a boolean (true/false).
// You can chain with else if and else for multiple conditions.
//
// https://www.learncpp.com/cpp-tutorial/if-statements-and-blocks/
// https://www.learncpp.com/cpp-tutorial/if-else-statements/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a function that returns a grade letter based on a score.
// - 90-100: return 'A'
// - 80-89: return 'B'
// - 70-79: return 'C'
// - 60-69: return 'D'
// - below 60: return 'F'
char getGrade(int score)
{
    if (score = 90)
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

// TODO: Implement a function that checks if a number is positive, negative, or zero.
// Return 1 for positive, -1 for negative, 0 for zero.
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
    return Catch::Session().run(argc, argv);
}

TEST_CASE("ControlFlow ControlFlow01")
{
    REQUIRE(getGrade(95) == 'A');
    REQUIRE(getGrade(85) == 'B');
    REQUIRE(getGrade(75) == 'C');
    REQUIRE(getGrade(65) == 'D');
    REQUIRE(getGrade(55) == 'F');

    REQUIRE(checkNumber(5) == 1);
    REQUIRE(checkNumber(-3) == -1);
    REQUIRE(checkNumber(0) == 0);
}
