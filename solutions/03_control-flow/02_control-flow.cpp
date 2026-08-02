// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// A switch statement provides a cleaner alternative to multiple if-else chains
// when comparing a single value against multiple constants.
//
// Key concepts:
// - The switch expression is compared against case constants
// - case constants must be constant expressions (known at compile time)
// - break prevents fall-through to the next case
// - default is optional and executes if no case matches
// - Cases can be grouped (case 1: case 2: same code)
//
// Important: Don't forget break!
// Without it, execution "falls through" to subsequent cases.
//
// https://www.learncpp.com/cpp-tutorial/switch-statements/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

// Returns the number of days in a given month.
// Uses switch for cleaner code than if-else chains.
int getDaysInMonth(int month)
{
    switch (month)
    {
    case 1:
        return 31;
    case 2:
        return 28;
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    default:
        return -1;
    }
}

// Returns the name of a day of the week.
// 1 = Monday, 2 = Tuesday, etc.
const char *getDayOfWeek(int day)
{
    switch (day)
    {
    case 1:
        return "Monday";
    case 2:
        return "Tuesday";
    case 3:
        return "Wednesday";
    case 4:
        return "Thursday";
    case 5:
        return "Friday";
    case 6:
        return "Saturday";
    case 7:
        return "Sunday";
    default:
        return "Invalid";
    }
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("ControlFlow ControlFlow02")
{
    REQUIRE(getDaysInMonth(1) == 31);
    REQUIRE(getDaysInMonth(2) == 28);
    REQUIRE(getDaysInMonth(4) == 30);
    REQUIRE(getDaysInMonth(12) == 31);
    REQUIRE(getDaysInMonth(0) == -1);

    REQUIRE(std::string(getDayOfWeek(1)) == "Monday");
    REQUIRE(std::string(getDayOfWeek(7)) == "Sunday");
    REQUIRE(std::string(getDayOfWeek(0)) == "Invalid");
}
