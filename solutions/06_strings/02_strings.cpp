// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::string is C++'s modern string class with automatic memory management.
//
// Key features:
// - Dynamic size (grows as needed)
// - Safe operations (no buffer overflows)
// - Rich set of methods
// - Can be used with streams (<<, >>)
//
// Common methods:
// - + : concatenation
// - ==, !=, <, >, <=, >= : comparison operators
// - length(), size() : number of characters
// - substr(pos, len) : extract substring
// - find(str) : find substring, returns npos if not found
// - find_first_of(chars) : find first occurrence of any character
// - empty() : check if string is empty
//
// Best practices:
// - Use std::string for most string operations
// - Use const std::string& for read-only parameters
// - Be aware of performance implications of frequent concatenations
//
// https://www.learncpp.com/cpp-tutorial/stdstring-overview/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

// Reverses a std::string.
std::string reverseString(const std::string& str) {
  std::string result;
  for (int i = str.length() - 1; i >= 0; i--) {
    result += str[i];
  }
  return result;
}

// Counts occurrences of a character in a string.
int countChar(const std::string& str, char c) {
  int count = 0;
  for (char ch : str) {
    if (ch == c) {
      count++;
    }
  }
  return count;
}

// Checks if a string is a palindrome (reads same forwards and backwards).
bool isPalindrome(const std::string& str) {
  int left = 0;
  int right = str.length() - 1;
  while (left < right) {
    if (str[left] != str[right]) {
      return false;
    }
    left++;
    right--;
  }
  return true;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Strings Strings02") {
  REQUIRE(reverseString("hello") == "olleh");
  REQUIRE(reverseString("a") == "a");
  REQUIRE(reverseString("racecar") == "racecar");

  REQUIRE(countChar("hello", 'l') == 2);
  REQUIRE(countChar("banana", 'a') == 3);
  REQUIRE(countChar("test", 'x') == 0);

  REQUIRE(isPalindrome("racecar"));
  REQUIRE(isPalindrome("a"));
  REQUIRE(isPalindrome("noon"));
  REQUIRE_FALSE(isPalindrome("hello"));
}
