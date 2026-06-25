// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Integer sizes and ranges:
// - int8_t, uint8_t: 8 bits
// - int16_t, uint16_t: 16 bits
// - int32_t, uint32_t: 32 bits
// - int64_t, uint64_t: 64 bits
//
// Fixed-width integers (from <cstdint>) guarantee specific sizes.
// Use them when you need precise control over data layout.
//
// Overflow behavior for unsigned: wraps around
// Overflow behavior for signed: undefined
//
// https://www.learncpp.com/cpp-tutorial/fundamental-data-types/
// https://www.learncpp.com/cpp-tutorial/unsigned-integers-and-why-to-avoid-them/

#include <climits>
#include <cstdint>
#include <gtest/gtest.h>

// TODO: Implement a function that calculates the sum of two unsigned 32-bit integers.
// Handle overflow by returning the maximum value if overflow occurs.
uint32_t addUint32(uint32_t a, uint32_t b)
{
    if (a > UINT32_MAX - b)
    {
        return UINT32_MAX;
    }
    return a + b;
}

// TODO: Implement a function that checks if an int8_t has overflowed
// when adding two values. Return true if overflow occurred.
bool hasOverflow(int8_t a, int8_t b)
{
    int16_t result = static_cast<int16_t>(a) + b;
    return result > INT8_MAX || result < INT8_MIN;
}

// TODO: Implement a function that converts Celsius to Fahrenheit.
// Formula: F = C * 9/5 + 32
double celsiusToFahrenheit(double celsius)
{
    return celsius * 9.0 / 5.0 + 32.0;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Primitives, Primitives03)
{
    ASSERT_EQ(addUint32(1, 2), 3);
    ASSERT_EQ(addUint32(UINT32_MAX, 1), UINT32_MAX);

    ASSERT_FALSE(hasOverflow(50, 50));
    ASSERT_TRUE(hasOverflow(100, 50));
    ASSERT_TRUE(hasOverflow(-100, -50));

    ASSERT_DOUBLE_EQ(celsiusToFahrenheit(0.0), 32.0);
    ASSERT_DOUBLE_EQ(celsiusToFahrenheit(100.0), 212.0);
}
