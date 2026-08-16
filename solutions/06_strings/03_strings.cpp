// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::string_view (C++17) is a non-owning view into a string.
//
// Why string_view?
// - Avoids unnecessary string copies
// - Works with both std::string and C-strings
// - Lightweight (just pointer + length)
// - Can improve performance in functions that only read strings
//
// Key characteristics:
// - Does NOT own the data
// - Original string must outlive the view
// - Read-only access (no modifications)
// - Can be used as function parameters for flexibility
//
// Common methods:
// - data(): pointer to underlying character array
// - size(), length(): number of characters
// - substr(pos, len): extract a substring (returns another view)
// - find(str): find substring
// - remove_prefix(n): advance start by n characters
// - remove_suffix(n): shrink length by n characters
//
// Best practices:
// - Use string_view for read-only string parameters
// - Be careful about lifetime (original must be valid)
// - Convert to string when you need ownership
//
// https://www.learncpp.com/cpp-tutorial/stdstring_view/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

// Returns the first word (characters before first space).
std::string_view firstWord(std::string_view sv) {
  size_t pos = sv.find(' ');
  if (pos == std::string_view::npos) {
    return sv;
  }
  return sv.substr(0, pos);
}

// Checks if a string_view starts with a given prefix.
bool startsWith(std::string_view sv, std::string_view prefix) {
  return sv.substr(0, prefix.length()) == prefix;
}

// Counts the number of words (separated by whitespace).
int countWords(std::string_view sv) {
  int count = 0;
  bool inWord = false;

  for (char c : sv) {
    if (c == ' ' || c == '\t' || c == '\n') {
      if (inWord) {
        count++;
        inWord = false;
      }
    } else {
      inWord = true;
    }
  }

  if (inWord) {
    count++;
  }

  return count;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Strings Strings03") {
  REQUIRE(firstWord("hello world") == "hello");
  REQUIRE(firstWord("single") == "single");
  REQUIRE(firstWord("") == "");

  REQUIRE(startsWith("hello", "he"));
  REQUIRE(startsWith("hello", "hello"));
  REQUIRE_FALSE(startsWith("hello", "world"));
  REQUIRE_FALSE(startsWith("", "anything"));

  REQUIRE(countWords("hello world") == 2);
  REQUIRE(countWords("one two three four") == 4);
  REQUIRE(countWords("single") == 1);
  REQUIRE(countWords("") == 0);
}
