// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ allows multiple functions to have the save name,
// this technique is called function overloading.
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-function-overloading/
// https://www.learncpp.com/cpp-tutorial/function-overload-differentiation/
// https://www.learncpp.com/cpp-tutorial/function-overload-resolution-and-ambiguous-matches/

#include <gtest/gtest.h>

// TODO: Add the appropriate functions.
// NOTE: Use the test cases as reference.
int add(int x, int y)
{
    return x + y;
}

float add(float x, float y)
{
    return x + y;
}

double add(double x, double y)
{
    return x + y;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Functions, Functions05)
{
    ASSERT_EQ(add(1, 1), 2);
    ASSERT_EQ(add(5.0f, 5.0f), 10.0f);
    ASSERT_EQ(add(100.555555, 100.555555), 201.11111);
}
