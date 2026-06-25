// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Fold expressions (C++17) provide a concise way to apply operations
// across all elements of a parameter pack.
//
// Four forms:
// 1. Unary left fold:  (... op pack)    -> ((pack1 op pack2) op ...) op packN
// 2. Unary right fold:  (pack op ...)    -> pack1 op (pack2 op (... op packN))
// 3. Binary left fold:  (init op ... op pack)  -> ((init op pack1) op ...) op packN
// 4. Binary right fold: (pack op ... op init)  -> pack1 op (... op (packN op init))
//
// Common use cases:
// - (... + args) : sum all arguments
// - (... && args) : logical AND all arguments
// - (... || args) : logical OR all arguments
// - (args + ...) : same as unary left for associative ops
//
// https://www.learncpp.com/cpp-tutorial/fold-expressions/

#include <gtest/gtest.h>
#include <iostream>

// TODO: Implement sum using a unary left fold expression.
// (... + args) adds all arguments together.
template <typename... Args> auto sum(Args... args)
{
    return (... + args);
}

// TODO: Implement printAll using a unary left fold.
// Print all arguments separated by spaces.
void printValue(int value)
{
    std::cout << value << " ";
}

template <typename T, typename... Args> void printAll(T first, Args... args)
{
    std::cout << first << " ";
    (..., (std::cout << args << " "));
}

// TODO: Implement a function that returns true if all arguments are positive.
template <typename... Args> bool allPositive(Args... args)
{
    return (... && (args > 0));
}

// TODO: Implement a function that returns the logical OR of all arguments.
template <typename... Args> bool anyTrue(Args... args)
{
    return (... || args);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Generics, Generics03)
{
    ASSERT_EQ(sum(1, 2, 3, 4, 5), 15);
    ASSERT_EQ(sum(1), 1);
    ASSERT_EQ(sum(1.5, 2.5, 3.0), 7.0);

    ASSERT_TRUE(allPositive(1, 2, 3));
    ASSERT_FALSE(allPositive(1, -2, 3));
    ASSERT_TRUE(allPositive(5));

    ASSERT_TRUE(anyTrue(false, true, false));
    ASSERT_FALSE(anyTrue(false, false, false));
    ASSERT_TRUE(anyTrue(true));
}
