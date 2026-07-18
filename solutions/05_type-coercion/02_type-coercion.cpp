// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Numeric promotions:
// - bool -> int (true becomes 1, false becomes 0)
// - char -> int (uses ASCII value)
// - short -> int
// - float -> double
//
// Arithmetic conversions (usual arithmetic conversions):
// - When binary operators have different types
// - Smaller types are promoted to larger types
// - Integer types are promoted to floating-point when needed
//
// Narrowing conversion risks:
// - Large values may wrap around
// - Fractional parts are lost
// - Always be aware of potential data loss
//
// https://www.learncpp.com/cpp-tutorial/arithmetic-conversions/

#include <gtest/gtest.h>

// Converts char to int (widening, always safe).
int charToInt(char c)
{
    return c;
}

// Converts int to char (narrowing, may lose data).
char intToChar(int i)
{
    return static_cast<char>(i);
}

// Checks if a double represents a whole number.
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
