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

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// TODO: Implement a function that sets the nth bit of a number.
int setBit(int num, int n) { return num | (1 << n); }

// TODO: Implement a function that clears the nth bit of a number.
int clearBit(int num, int n) { return num & ~(1 << n); }

// TODO: Implement a function that toggles the nth bit of a number.
int toggleBit(int num, int n) { return num ^ (1 << n); }

// TODO: Implement a function that checks if the nth bit is set.
bool isBitSet(int num, int n) { return (num & (1 << n)) != 0; }

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("BitManipulation BitManipulation01") {
  REQUIRE(setBit(0, 0) == 1);
  REQUIRE(setBit(4, 1) == 6);

  REQUIRE(clearBit(7, 0) == 6);
  REQUIRE(clearBit(8, 3) == 0);

  REQUIRE(toggleBit(5, 0) == 4);
  REQUIRE(toggleBit(4, 0) == 5);

  REQUIRE(isBitSet(5, 0));
  REQUIRE_FALSE(isBitSet(5, 1));
}
