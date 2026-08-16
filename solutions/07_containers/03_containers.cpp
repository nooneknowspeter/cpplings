// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::map stores key-value pairs sorted by key.
//
// Key characteristics:
// - Sorted by key (uses std::less, which calls operator<)
// - Unique keys (no duplicate keys)
// - O(log n) for insertion, deletion, lookup
// - Implemented as a red-black tree
// - Each element is a std::pair<const Key, Value>
//
// Common operations:
// - insert({key, value}): insert key-value pair
// - find(key): find element, returns end() if not found
// - erase(key): remove element by key
// - count(key): returns 1 if key exists, 0 otherwise
// - operator[key]: access or create element (careful: creates entry!)
// - at(key): access element, throws if not found
//
// When to use map:
// - Need sorted data or ordered iteration
// - Need key-value associations
// - When O(log n) operations are acceptable
//
// https://www.learncpp.com/cpp-tutorial/stdmap/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>
#include <vector>

// Creates a simple phonebook.
std::map<std::string, int> createPhonebook() {
  std::map<std::string, int> phonebook;
  phonebook["Alice"] = 123;
  phonebook["Bob"] = 456;
  phonebook["Charlie"] = 789;
  return phonebook;
}

// Looks up a name in the phonebook.
int lookup(const std::map<std::string, int>& phonebook,
           const std::string& name) {
  auto it = phonebook.find(name);
  if (it != phonebook.end()) {
    return it->second;
  }
  return -1;
}

// Counts word occurrences using a map.
std::map<std::string, int> wordCount(const std::vector<std::string>& words) {
  std::map<std::string, int> counts;
  for (const std::string& word : words) {
    counts[word]++;
  }
  return counts;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Containers Containers03") {
  auto phonebook = createPhonebook();
  REQUIRE(phonebook.size() == 3);
  REQUIRE(lookup(phonebook, "Alice") == 123);
  REQUIRE(lookup(phonebook, "Unknown") == -1);

  std::vector<std::string> words = {"apple",  "banana", "apple",
                                    "cherry", "banana", "apple"};
  auto counts = wordCount(words);
  REQUIRE(counts["apple"] == 3);
  REQUIRE(counts["banana"] == 2);
  REQUIRE(counts["cherry"] == 1);
}
