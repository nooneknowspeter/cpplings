// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ has several fundamental data types for storing different kinds of values.
//
// Integer types (whole numbers):
// - int: typically 32 bits, range: -2,147,483,648 to 2,147,483,647
// - short: typically 16 bits
// - long: at least 32 bits
// - long long: at least 64 bits
//
// Floating-point types (decimal numbers):
// - float: typically 32 bits, ~6-7 significant digits
// - double: typically 64 bits, ~15 significant digits
// - long double: even more precision
//
// The 'auto' keyword lets the compiler deduce the type.
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-fundamental-data-types/
// https://www.learncpp.com/cpp-tutorial/floating-point-numbers/

#include <cstdint>
#include <gtest/gtest.h>

// TODO: Declare an int variable named 'age' with value 25.
int age = 25;

// TODO: Declare a double variable named 'price' with value 19.99.
double price = 19.99;

// TODO: Declare a const int named 'MAX_SIZE' with value 100.
const int MAX_SIZE = 100;

// TODO: Use 'auto' to declare a variable 'quantity' with value 42.
// The compiler will deduce it as int.
auto quantity = 42;

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Primitives, Primitives01)
{
    ASSERT_EQ(age, 25);
    ASSERT_DOUBLE_EQ(price, 19.99);
    ASSERT_EQ(MAX_SIZE, 100);
    ASSERT_EQ(quantity, 42);
}
