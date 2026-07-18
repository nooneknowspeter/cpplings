// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Header files organize code and enable code reuse.
//
// #include copies the header content into your file.
// Headers typically contain:
// - Declarations (function prototypes, class definitions)
// - Constants
// - Template definitions
//
// Best practices:
// - Use #ifndef/#define/#endif guards to prevent multiple inclusion
// - Separate declarations (.h) from implementations (.cpp)
// - Include only what's needed
//
// https://www.learncpp.com/cpp-tutorial/header-files/

#include <gtest/gtest.h>

// TODO: Create a function declaration for add.
// The implementation is in src/math_utils.cpp
int add(int a, int b);

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Headers, Headers01)
{
    ASSERT_EQ(add(2, 3), 5);
    ASSERT_EQ(add(-1, 1), 0);
    ASSERT_EQ(add(0, 0), 0);
}
