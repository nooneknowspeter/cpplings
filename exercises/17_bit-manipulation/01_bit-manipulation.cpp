// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Bit manipulation operates on individual bits.
//
// Bitwise operators:
// - & : AND (1 only if both bits are 1)
// - | : OR (1 if either bit is 1)
// - ^ : XOR (1 if bits are different)
// - ~ : NOT (flips all bits)
// - <<: left shift (multiply by 2^n)
// - >>: right shift (divide by 2^n)
//
// Common uses:
// - Flags and bitmasks
// - Efficient arithmetic
// - Hardware register access
//
// https://www.learncpp.com/cpp-tutorial/bitwise-operators/

#include <gtest/gtest.h>

// TODO: Implement a function that sets the nth bit of a number.
int setBit(int num, int n)
{
    return num | (1 << n);
}

// TODO: Implement a function that clears the nth bit of a number.
int clearBit(int num, int n)
{
    return num & ~(1 << n);
}

// TODO: Implement a function that toggles the nth bit of a number.
int toggleBit(int num, int n)
{
    return num ^ (1 << n);
}

// TODO: Implement a function that checks if the nth bit is set.
bool isBitSet(int num, int n)
{
    return (num & (1 << n)) != 0;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(BitManipulation, BitManipulation01)
{
    ASSERT_EQ(setBit(0, 0), 1);
    ASSERT_EQ(setBit(4, 1), 6);

    ASSERT_EQ(clearBit(7, 0), 6);
    ASSERT_EQ(clearBit(8, 3), 0);

    ASSERT_EQ(toggleBit(5, 0), 4);
    ASSERT_EQ(toggleBit(4, 0), 5);

    ASSERT_TRUE(isBitSet(5, 0));
    ASSERT_FALSE(isBitSet(5, 1));
}
