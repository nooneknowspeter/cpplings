// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Type deduction with auto:
// - auto deduces the type from the initializer
// - References are dropped unless & is used
// - const and volatile are dropped unless explicitly kept
//
// constexpr: compile-time constant evaluation
// - Value is computed at compile time
// - Can be used in contexts requiring constants
//
// https://www.learncpp.com/cpp-tutorial/type-deduction-with-auto/
// https://www.learncpp.com/cpp-tutorial/constexpr-variables/

#include <gtest/gtest.h>

// TODO: Use auto to deduce the type of a value.
// Return the result of adding an int and a double.
auto addWithAuto(int a, double b)
{
    auto result = a + b;
    return result;
}

// TODO: Create a constexpr function that calculates the square of a number.
// This should be evaluable at compile time.
constexpr int square(int x)
{
    return x * x;
}

// TODO: Create a constexpr variable that stores the result of square(10).
// This will be computed at compile time.
constexpr int SQUARED_TEN = square(10);

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TypeCoercion, TypeCoercion03)
{
    ASSERT_DOUBLE_EQ(addWithAuto(3, 4.5), 7.5);

    ASSERT_EQ(square(5), 25);
    ASSERT_EQ(square(-3), 9);
    ASSERT_EQ(square(0), 0);

    ASSERT_EQ(SQUARED_TEN, 100);
}
