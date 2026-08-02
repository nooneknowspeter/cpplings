// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::unordered_map is a hash table implementation of map.
//
// Key features:
// - O(1) average case lookup, insertion, deletion
// - No guaranteed order of elements
// - Requires hash function for keys
// - Works with any hashable type
//
// std::unordered_set is a hash table implementation of set.
//
// When to use unordered vs ordered:
// - unordered_map/set: when you need fast lookups and don't care about order
// - map/set: when you need sorted data or ordered iteration
//
// https://www.learncpp.com/cpp-tutorial/stdunordered_map/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include <unordered_set>

// TODO: Implement a function that counts word frequencies using unordered_map.
std::unordered_map<std::string, int> wordFrequency(const std::vector<std::string> &words)
{
    std::unordered_map<std::string, int> freq;
    for (const std::string &word : words)
    {
        freq[word]++;
    }
    return freq;
}

// TODO: Implement a function that finds unique elements using unordered_set.
std::unordered_set<int> uniqueElements(const std::vector<int> &v)
{
    return std::unordered_set<int>(v.begin(), v.end());
}

// TODO: Implement a function that checks if two unordered_maps are equal.
bool mapsEqual(const std::unordered_map<std::string, int> &a, const std::unordered_map<std::string, int> &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (const auto &[key, value] : a)
    {
        auto it = b.find(key);
        if (it == b.end() || it->second != value)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Containers Containers05")
{
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
