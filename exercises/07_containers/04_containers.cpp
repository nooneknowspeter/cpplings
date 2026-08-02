// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::set is a container that stores unique elements in sorted order.
//
// Key features:
// - No duplicates (automatically ignored)
// - Sorted (uses operator<)
// - O(log n) insertion, deletion, lookup
// - Good for membership testing and deduplication
//
// Common operations:
// - insert(val): add element
// - find(val): find element
// - count(val): returns 1 if exists, 0 otherwise
// - erase(val): remove element
// - empty(): check if empty
// - size(): number of elements
//
// https://www.learncpp.com/cpp-tutorial/stdset/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <set>

// TODO: Implement a function that checks if a set contains an element.
bool contains(const std::set<int> &s, int value)
{
    return s.find(value) != s.end();
}

// TODO: Implement a function that returns the intersection of two sets.
// (Elements that exist in both sets)
std::set<int> setIntersection(const std::set<int> &a, const std::set<int> &b)
{
    std::set<int> result;
    for (int val : a)
    {
        if (b.find(val) != b.end())
        {
            result.insert(val);
        }
    }
    return result;
}

// TODO: Implement a function that returns the union of two sets.
// (All unique elements from both sets)
std::set<int> setUnion(const std::set<int> &a, const std::set<int> &b)
{
    std::set<int> result = a;
    for (int val : b)
    {
        result.insert(val);
    }
    return result;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Containers Containers04")
{
    std::set<int> s = {1, 2, 3, 4, 5};

    REQUIRE(contains(s, 3));
    REQUIRE_FALSE(contains(s, 10));

    std::set<int> a = {1, 2, 3};
    std::set<int> b = {2, 3, 4};
    std::set<int> intersection = {2, 3};
    REQUIRE(setIntersection(a, b) == intersection);

    std::set<int> unionSet = {1, 2, 3, 4};
    REQUIRE(setUnion(a, b) == unionSet);
}
