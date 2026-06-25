// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Function templates allow writing code that works with any data type.
//
// Key concepts:
// - template <typename T>: declares a type parameter
// - T is a placeholder that gets replaced with actual types
// - The compiler generates specialized code for each type used
// - This is called template instantiation
//
// Benefits of templates:
// - Code reusability
// - Type safety (with proper constraints)
// - Performance (no runtime overhead)
//
// When to use templates:
// - When the algorithm is the same for different types
// - When you need type-safe containers
// - When you want to avoid code duplication
//
// https://www.learncpp.com/cpp-tutorial/function-templates/

#include <gtest/gtest.h>

// Returns the maximum of two values.
template <typename T> T maximum(T a, T b)
{
    return (a > b) ? a : b;
}

// Swaps two values.
template <typename T> void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Returns the absolute value of a number.
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
