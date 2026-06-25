// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Type coercion (type conversion) is converting one type to another.
//
// Implicit conversion (coercion):
// - Done automatically by the compiler
// - Usually widening (small type -> large type)
// - int to double, char to int, etc.
//
// Explicit conversion (casting) using static_cast:
// - Compile-time checked conversion
// - Makes intent clear
// - Use when conversion might lose data
//
// Why explicit casting?
// - Makes programmer's intent clear
// - Helps catch accidental conversions
// - Easier to find in code reviews
//
// https://www.learncpp.com/cpp-tutorial/implicit-type-conversion-coercion/
// https://www.learncpp.com/cpp-tutorial/explicit-type-conversion-casting/

#include <gtest/gtest.h>

// Adds an int and a double.
// The int is implicitly converted to double for the addition.
double addIntDouble(int a, double b)
{
    return a + b;
}

// Divides two integers and returns a double.
// Must explicitly cast to prevent integer division.
double divideInts(int a, int b)
{
    return static_cast<double>(a) / static_cast<double>(b);
}

// Truncates a double to an int (rounds toward zero).
int truncateToInt(double d)
{
    return static_cast<int>(d);
}

// Rounds a double to the nearest integer.
int roundToInt(double d)
{
    if (d >= 0)
    {
        return static_cast<int>(d + 0.5);
    }
    else
    {
        return static_cast<int>(d - 0.5);
    }
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TypeCoercion, TypeCoercion01)
{
    ASSERT_DOUBLE_EQ(addIntDouble(5, 3.5), 8.5);
    ASSERT_DOUBLE_EQ(addIntDouble(10, 2.0), 12.0);

    ASSERT_DOUBLE_EQ(divideInts(10, 4), 2.5);
    ASSERT_DOUBLE_EQ(divideInts(7, 2), 3.5);

    ASSERT_EQ(truncateToInt(3.7), 3);
    ASSERT_EQ(truncateToInt(3.2), 3);

    ASSERT_EQ(roundToInt(3.7), 4);
    ASSERT_EQ(roundToInt(3.2), 3);
    ASSERT_EQ(roundToInt(-3.7), -4);
}
