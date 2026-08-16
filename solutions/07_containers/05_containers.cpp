// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::unordered_map is a hash table (key-value pairs).
//
// Key characteristics:
// - O(1) average case for insert, find, erase
// - No guaranteed order of elements
// - Requires hash function for key type
// - Better performance for large datasets
//
// std::unordered_set is a hash table (unique elements).
//
// When to use unordered containers:
// - Fast lookups are more important than order
// - Large datasets (O(1) beats O(log n))
// - When you don't need sorted iteration
//
// When to use ordered containers:
// - Need sorted data or ordered iteration
// - Need predictable iteration order
// - Small datasets where O(log n) is fine
//
// https://www.learncpp.com/cpp-tutorial/stdunordered_map/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Counts word frequencies using unordered_map.
std::unordered_map<std::string, int> wordFrequency(
    const std::vector<std::string>& words) {
  std::unordered_map<std::string, int> freq;
  for (const std::string& word : words) {
    freq[word]++;
  }
  return freq;
}

// Returns unique elements from a vector using unordered_set.
std::unordered_set<int> uniqueElements(const std::vector<int>& v) {
  return std::unordered_set<int>(v.begin(), v.end());
}

// Checks if two unordered_maps have the same key-value pairs.
bool mapsEqual(const std::unordered_map<std::string, int>& a,
               const std::unordered_map<std::string, int>& b) {
  if (a.size() != b.size()) {
    return false;
  }
  for (const auto& [key, value] : a) {
    auto it = b.find(key);
    if (it == b.end() || it->second != value) {
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Containers Containers05") {
  std::vector<std::string> words = {"a", "b", "a", "c", "b", "a"};
  auto freq = wordFrequency(words);
  REQUIRE(freq["a"] == 3);
  REQUIRE(freq["b"] == 2);
  REQUIRE(freq["c"] == 1);

  std::vector<int> v = {1, 2, 3, 2, 1};
  auto unique = uniqueElements(v);
  REQUIRE(unique.size() == 3);

  std::unordered_map<std::string, int> m1 = {{"a", 1}, {"b", 2}};
  std::unordered_map<std::string, int> m2 = {{"b", 2}, {"a", 1}};
  std::unordered_map<std::string, int> m3 = {{"a", 1}, {"b", 3}};
  REQUIRE(mapsEqual(m1, m2));
  REQUIRE_FALSE(mapsEqual(m1, m3));
}
