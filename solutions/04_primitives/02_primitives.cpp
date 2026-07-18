// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Boolean type (bool):
// - Represents true/false values
// - Size: typically 1 byte
// - Values: true (1) or false (0)
// - When printed, shows 1 or 0
//
// Character type (char):
// - Represents a single character
// - Size: typically 1 byte (8 bits)
// - Stored as ASCII value
// - Use single quotes for char literals: 'A', '5', '\n'
//
// Key concepts:
// - bool can be used in conditions directly
// - char comparisons use ASCII values
// - std::tolower/toupper for case-insensitive comparison
//
// https://www.learncpp.com/cpp-tutorial/boolean-values/
// https://www.learncpp.com/cpp-tutorial/char/

#include <cctype>
#include <gtest/gtest.h>

// Checks if a character is a vowel (case-insensitive).
bool isVowel(char c)
{
    c = std::tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

// Checks if a character is a digit ('0' through '9').
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// Converts a boolean to a string.
const char *boolToString(bool b)
{
    return b ? "true" : "false";
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Primitives, Primitives02)
{
    ASSERT_TRUE(isVowel('a'));
    ASSERT_TRUE(isVowel('E'));
    ASSERT_FALSE(isVowel('b'));
    ASSERT_FALSE(isVowel('Z'));

    ASSERT_TRUE(isDigit('5'));
    ASSERT_TRUE(isDigit('0'));
    ASSERT_FALSE(isDigit('a'));
    ASSERT_FALSE(isDigit('#'));

    ASSERT_STREQ(boolToString(true), "true");
    ASSERT_STREQ(boolToString(false), "false");
}
