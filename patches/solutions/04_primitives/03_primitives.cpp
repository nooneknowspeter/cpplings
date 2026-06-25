// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Fixed-width integer types (from <cstdint>):
// - int8_t, uint8_t: 8 bits
// - int16_t, uint16_t: 16 bits
// - int32_t, uint32_t: 32 bits
// - int64_t, uint64_t: 64 bits
//
// Use these when you need:
// - Predictable sizes for file formats or network protocols
// - Cross-platform compatibility
// - Bit-level operations
//
// Overflow behavior:
// - unsigned: wraps around (e.g., UINT8_MAX + 1 = 0)
// - signed: undefined behavior (avoid!)
//
// Tips:
// - Use unsigned for bit manipulation and when negatives don't make sense
// - Prefer standard int for general calculations
// - Be careful when mixing signed and unsigned
//
// https://www.learncpp.com/cpp-tutorial/fundamental-data-types/
// https://www.learncpp.com/cpp-tutorial/unsigned-integers-and-why-to-avoid-them/

#include <climits>
#include <cstdint>
#include <gtest/gtest.h>

// Adds two uint32_t values with overflow detection.
uint32_t addUint32(uint32_t a, uint32_t b)
{
    if (a > UINT32_MAX - b)
    {
        return UINT32_MAX;
    }
    return a + b;
}

// Checks if adding two int8_t values would overflow.
bool hasOverflow(int8_t a, int8_t b)
{
    int16_t result = static_cast<int16_t>(a) + b;
    return result > INT8_MAX || result < INT8_MIN;
}

// Converts Celsius to Fahrenheit.
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
