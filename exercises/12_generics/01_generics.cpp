// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Function templates allow writing generic functions that work with any type.
//
// Syntax:
// template <typename T>
// return_type functionName(T param) { ... }
//
// The compiler generates specific code for each type used.
// This is called template instantiation.
//
// https://www.learncpp.com/cpp-tutorial/function-templates/

#include <gtest/gtest.h>

// TODO: Implement a template function that returns the maximum of two values.
// Use if-else or the ternary operator.
template <typename T> T maximum(T a, T b)
{
    return (a > b) ? a : b;
}

// TODO: Implement a template function that swaps two values.
template <typename T> void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// TODO: Implement a template function that returns the absolute value.
template <typename T> T absolute(T value)
{
    if (value < T{})
    {
        return -value;
    }
    return value;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Generics, Generics01)
{
    ASSERT_EQ(maximum(5, 10), 10);
    ASSERT_EQ(maximum(3.14, 2.71), 3.14);
    ASSERT_EQ(maximum('a', 'z'), 'z');

    int x = 5, y = 10;
    swap(x, y);
    ASSERT_EQ(x, 10);
    ASSERT_EQ(y, 5);

    ASSERT_EQ(absolute(-5), 5);
    ASSERT_EQ(absolute(5), 5);
    ASSERT_EQ(absolute(-3.14), 3.14);
}
