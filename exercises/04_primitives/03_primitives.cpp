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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <cstdint>

// TODO: Implement a function that calculates the sum of two unsigned 32-bit
// integers. Handle overflow by returning the maximum value if overflow occurs.
uint32_t addUint32(uint32_t a, uint32_t b) {
  if (a > UINT32_MAX - b) {
    return UINT32_MAX;
  }
  return a + b;
}

// TODO: Implement a function that checks if an int8_t has overflowed
// when adding two values. Return true if overflow occurred.
bool hasOverflow(int8_t a, int8_t b) {
  int16_t result = static_cast<int16_t>(a) + b;
  return result > INT8_MAX || result < INT8_MIN;
}

// TODO: Implement a function that converts Celsius to Fahrenheit.
// Formula: F = C * 9/5 + 32
celsiusToFahrenheit(double celsius) { return celsius * 9.0 / 5.0 + 32.0; }

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Primitives Primitives03") {
  REQUIRE(addUint32(1, 2) == 3);
  REQUIRE(addUint32(UINT32_MAX, 1) == UINT32_MAX);

  REQUIRE_FALSE(hasOverflow(50, 50));
  REQUIRE(hasOverflow(100, 50));
  REQUIRE(hasOverflow(-100, -50));

  REQUIRE(celsiusToFahrenheit(0.0) == Catch::Approx(32.0));
  REQUIRE(celsiusToFahrenheit(100.0) == Catch::Approx(212.0));
}
