// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Numeric promotions and conversions:
// - bool -> int
// - char -> int
// - short -> int
// - float -> double
//
// When mixing types in expressions, the compiler promotes smaller
// types to larger ones (arithmetic conversions).
//
// Narrowing conversion: converting a larger type to a smaller one
// Can lose data - use with caution!
//
// https://www.learncpp.com/cpp-tutorial/arithmetic-conversions/

#include <gtest/gtest.h>

// TODO: Implement a function that safely converts a char to an int.
// This is a widening conversion (char -> int), no data loss.
int charToInt(char c)
{
    return c;
}

// TODO: Implement a function that converts an int to a char.
// This is a narrowing conversion - use static_cast to make it explicit.
char intToChar(int i)
{
    return static_cast<char>(i);
}

// TODO: Implement a function that checks if a double can be safely
// converted to an int without losing fractional part.
bool isWholeNumber(double d)
{
    int i = static_cast<int>(d);
    return static_cast<double>(i) == d;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TypeCoercion, TypeCoercion02)
{
    ASSERT_EQ(charToInt('A'), 65);
    ASSERT_EQ(charToInt('0'), 48);

    ASSERT_EQ(intToChar(65), 'A');
    ASSERT_EQ(intToChar(97), 'a');

    ASSERT_TRUE(isWholeNumber(5.0));
    ASSERT_TRUE(isWholeNumber(-3.0));
    ASSERT_FALSE(isWholeNumber(5.5));
    ASSERT_FALSE(isWholeNumber(-3.7));
}
