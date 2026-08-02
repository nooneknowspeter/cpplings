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

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

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
    return Catch::Session().run(argc, argv);
}

TEST_CASE("TypeCoercion TypeCoercion02")
{
    REQUIRE(charToInt('A') == 65);
    REQUIRE(charToInt('0') == 48);

    REQUIRE(intToChar(65) == 'A');
    REQUIRE(intToChar(97) == 'a');

    REQUIRE(isWholeNumber(5.0));
    REQUIRE(isWholeNumber(-3.0));
    REQUIRE_FALSE(isWholeNumber(5.5));
    REQUIRE_FALSE(isWholeNumber(-3.7));
}
