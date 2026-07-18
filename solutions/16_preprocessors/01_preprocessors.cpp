// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// The preprocessor processes code before compilation.
//
// Common directives:
// - #include: include header files
// - #define: define macros
// - #ifdef/#ifndef: conditional compilation
// - #endif: end conditional block
//
// Header guards prevent multiple inclusion:
// #ifndef HEADER_H
// #define HEADER_H
// ... content ...
// #endif
//
// https://www.learncpp.com/cpp-tutorial/header-files/

#include <gtest/gtest.h>

// TODO: Define a macro SQUARE that squares a number.
#define SQUARE(x) ((x) * (x))

// TODO: Define a macro MAX that returns the maximum of two values.
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// TODO: Define a macro IS_EVEN that returns true if a number is even.
#define IS_EVEN(x) ((x) % 2 == 0)

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Preprocessors, Preprocessors01)
{
    ASSERT_EQ(SQUARE(5), 25);
    ASSERT_EQ(SQUARE(-3), 9);

    ASSERT_EQ(MAX(3, 5), 5);
    ASSERT_EQ(MAX(10, 2), 10);

    ASSERT_TRUE(IS_EVEN(4));
    ASSERT_FALSE(IS_EVEN(5));
}
