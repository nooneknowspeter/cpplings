// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// A switch statement provides a cleaner way to compare a value
// against multiple possible matches.
//
// Syntax:
// switch (value) {
//     case constant1:
//         // code
//         break;
//     case constant2:
//         // code
//         break;
//     default:
//         // code if no case matches
// }
//
// Important: Don't forget the 'break' statement!
// Without it, execution will "fall through" to the next case.
//
// https://www.learncpp.com/cpp-tutorial/switch-statements/

#include <gtest/gtest.h>

// TODO: Implement a function that returns the number of days in a month.
// Use a switch statement.
// January: 31, February: 28, March: 31, April: 30,
// May: 31, June: 30, July: 31, August: 31,
// September: 30, October: 31, November: 30, December: 31
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

// TODO: Implement a function that returns the day of the week as a string.
// Use a switch statement.
// 1: "Monday", 2: "Tuesday", 3: "Wednesday", 4: "Thursday",
// 5: "Friday", 6: "Saturday", 7: "Sunday"
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ControlFlow, ControlFlow02)
{
    ASSERT_EQ(getDaysInMonth(1), 31);
    ASSERT_EQ(getDaysInMonth(2), 28);
    ASSERT_EQ(getDaysInMonth(4), 30);
    ASSERT_EQ(getDaysInMonth(12), 31);
    ASSERT_EQ(getDaysInMonth(0), -1);

    ASSERT_STREQ(getDayOfWeek(1), "Monday");
    ASSERT_STREQ(getDayOfWeek(7), "Sunday");
    ASSERT_STREQ(getDayOfWeek(0), "Invalid");
}
