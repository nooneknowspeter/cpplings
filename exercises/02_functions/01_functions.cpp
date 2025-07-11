//
//
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-functions/
// https://www.learncpp.com/cpp-tutorial/function-return-values-value-returning-functions/

#include <gtest/gtest.h>
#include <iostream>

int function01(int x)
{
    int y{x + 1};

    return y;
}

TEST(Functions, Function01)
{
    ASSERT_EQ(function01(0), 1);
    ASSERT_EQ(function01(1), 2);
    ASSERT_EQ(function01(2), 3);
};
