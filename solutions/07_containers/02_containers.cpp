// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C-style arrays vs std::array:
//
// C-style: int arr[5] = {1, 2, 3, 4, 5};
// - Fixed size but decays to pointer when passed to functions
// - No bounds checking
// - Manual memory management
// - Limited standard library support
//
// std::array: std::array<int, 5> arr = {1, 2, 3, 4, 5};
// - Fixed size at compile time (size is part of the type)
// - Does NOT decay to pointer (preserves size information)
// - Provides STL-like interface (iterators, algorithms)
// - Safer: use .at() for bounds-checked access
// - Stack-allocated (no dynamic memory)
//
// Best practices:
// - Prefer std::array for fixed-size arrays
// - Use C-style arrays only for C interop
// - Always know your array size at compile time
//
// https://www.learncpp.com/cpp-tutorial/stdarray/

#include <array>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>

// Sums all elements in a std::array.
int sumArray(const std::array<int, 5>& arr) {
  int sum = 0;
  for (int num : arr) {
    sum += num;
  }
  return sum;
}

// Finds an element in the array.
// Returns its index, or -1 if not found.
int findInArray(const std::array<int, 5>& arr, int target) {
  for (int i = 0; i < 5; i++) {
    if (arr[i] == target) {
      return i;
    }
  }
  return -1;
}

// Doubles each element in the array.
std::array<int, 5> doubleArray(const std::array<int, 5>& arr) {
  std::array<int, 5> result;
  for (int i = 0; i < 5; i++) {
    result[i] = arr[i] * 2;
  }
  return result;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Containers Containers02") {
  std::array<int, 5> arr = {1, 2, 3, 4, 5};
  REQUIRE(sumArray(arr) == 15);

  REQUIRE(findInArray(arr, 3) == 2);
  REQUIRE(findInArray(arr, 6) == -1);

  std::array<int, 5> doubled = {2, 4, 6, 8, 10};
  REQUIRE(doubleArray(arr) == doubled);
}
