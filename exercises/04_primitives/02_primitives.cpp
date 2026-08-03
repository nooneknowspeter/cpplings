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
// https://www.learncpp.com/cpp-tutorial/boolean-values/
// https://www.learncpp.com/cpp-tutorial/char/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <string>

// TODO: Implement a function that checks if a character is a vowel.
// Return true if the character is 'a', 'e', 'i', 'o', 'u' (case insensitive).
bool isVowel(char c)
{
    c = std::tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

// TODO: Implement a function that checks if a character is a digit.
// Return true if the character is '0' through '9'.
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// TODO: Implement a function that converts a boolean to a string.
// Return "true" if b is true, "false" otherwise.
const char *boolToString(bool b)
{
    b ? "true" : "false";
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Primitives Primitives02")
{
    REQUIRE(isVowel('a'));
    REQUIRE(isVowel('E'));
    REQUIRE_FALSE(isVowel('b'));
    REQUIRE_FALSE(isVowel('Z'));

    REQUIRE(isDigit('5'));
    REQUIRE(isDigit('0'));
    REQUIRE_FALSE(isDigit('a'));
    REQUIRE_FALSE(isDigit('#'));

    REQUIRE(std::string(boolToString(true)) == "true");
    REQUIRE(std::string(boolToString(false)) == "false");
}
