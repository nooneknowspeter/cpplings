// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::map is an associative container that stores key-value pairs.
// Elements are sorted by key.
//
// Key features:
// - Sorted by key (uses operator<)
// - Unique keys (no duplicates)
// - O(log n) insertion, deletion, lookup
// - Red-black tree implementation
//
// Common operations:
// - insert({key, value}): add key-value pair
// - find(key): find element, returns end() if not found
// - erase(key): remove element
// - count(key): returns 1 if key exists, 0 otherwise
// - operator[key]: access/create element (creates if not exists)
//
// https://www.learncpp.com/cpp-tutorial/stdmap/

#include <gtest/gtest.h>
#include <map>
#include <string>

// TODO: Implement a function that creates a simple phonebook map.
// Return a map with: "Alice" -> 123, "Bob" -> 456, "Charlie" -> 789.
std::map<std::string, int> createPhonebook()
{
    std::map<std::string, int> phonebook;
    phonebook["Alice"] = 123;
    phonebook["Bob"] = 456;
    phonebook["Charlie"] = 789;
    return phonebook;
}

// TODO: Implement a function that looks up a name in the phonebook.
// Return the number if found, or -1 if not found.
int lookup(const std::map<std::string, int> &phonebook, const std::string &name)
{
    auto it = phonebook.find(name);
    if (it != phonebook.end())
    {
        return it->second;
    }
    return -1;
}

// TODO: Implement a function that counts occurrences of words.
// Return a map from word to count.
std::map<std::string, int> wordCount(const std::vector<std::string> &words)
{
    std::map<std::string, int> counts;
    for (const std::string &word : words)
    {
        counts[word]++;
    }
    return counts;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Containers, Containers03)
{
    auto phonebook = createPhonebook();
    ASSERT_EQ(phonebook.size(), 3);
    ASSERT_EQ(lookup(phonebook, "Alice"), 123);
    ASSERT_EQ(lookup(phonebook, "Unknown"), -1);

    std::vector<std::string> words = {"apple", "banana", "apple", "cherry", "banana", "apple"};
    auto counts = wordCount(words);
    ASSERT_EQ(counts["apple"], 3);
    ASSERT_EQ(counts["banana"], 2);
    ASSERT_EQ(counts["cherry"], 1);
}
