// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Exceptions provide a way to handle errors gracefully.
//
// Key concepts:
// - throw: raises an exception
// - try/catch: catches and handles exceptions
// - catch (...) catches all exceptions
// - Exceptions should be used for exceptional conditions, not control flow
//
// Standard exceptions:
// - std::runtime_error, std::logic_error
// - std::out_of_range, std::invalid_argument
//
// https://www.learncpp.com/cpp-tutorial/intro-to-exceptions/

#include <gtest/gtest.h>
#include <stdexcept>

// TODO: Implement a function that divides two numbers.
// Throw std::runtime_error if dividing by zero.
double divide(double a, double b)
{
    if (b == 0.0)
    {
        throw std::runtime_error("Division by zero");
    }
    return a / b;
}

// TODO: Implement a function that returns the element at index in an array.
// Throw std::out_of_range if index is out of bounds.
int getElement(const int *arr, int size, int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Index out of bounds");
    }
    return arr[index];
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Exceptions, Exceptions01)
{
    ASSERT_DOUBLE_EQ(divide(10.0, 2.0), 5.0);
    ASSERT_DOUBLE_EQ(divide(7.0, 3.0), 2.3333333333333335);

    int arr[] = {1, 2, 3, 4, 5};
    ASSERT_EQ(getElement(arr, 5, 0), 1);
    ASSERT_EQ(getElement(arr, 5, 4), 5);
}
