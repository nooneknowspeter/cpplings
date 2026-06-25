// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// A lambda expression (also known as a lambda or anonymous function)
// is a shorthand way to define a function without giving it a name.
//
// Lambda syntax: [capture clause](parameters) -> return_type { body }
//
// - Capture clause []: specifies which variables from the enclosing scope to capture
// - Parameters (): like regular function parameters
// - Return type ->: can be omitted for type inference
// - Body {}: the actual code
//
// Common capture modes:
// [] - capture nothing
// [=] - capture everything by value (copy)
// [&] - capture everything by reference
// [x, &y] - capture x by value, y by reference
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/
// https://www.learncpp.com/cpp-tutorial/lambda-captures/

#include <gtest/gtest.h>
#include <vector>

// TODO: Create a lambda named 'isEven' that returns true if a number is even.
// The lambda should take an int parameter and return a bool.
auto isEven = [](int n) -> bool { return n % 2 == 0; };

// TODO: Create a lambda named 'doubleIt' that doubles a number.
// Use the 'auto' keyword for the return type inference.
auto doubleIt = [](int n) { return n * 2; };

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Functions, Functions08)
{
    ASSERT_TRUE(isEven(2));
    ASSERT_TRUE(isEven(4));
    ASSERT_FALSE(isEven(3));
    ASSERT_FALSE(isEven(5));

    ASSERT_EQ(doubleIt(5), 10);
    ASSERT_EQ(doubleIt(0), 0);
    ASSERT_EQ(doubleIt(-3), -6);
}

TEST(Functions, Functions08_Capture)
{
    // TODO: Create a lambda that captures 'multiplier' by value.
    const int multiplier = 3;
    auto multiplyByGlobal = [=](int n) { return n * multiplier; };
    ASSERT_EQ(multiplyByGlobal(4), 12);
}
