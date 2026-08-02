// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::set stores unique elements in sorted order.
//
// Key characteristics:
// - No duplicates (inserting a duplicate does nothing)
// - Sorted (uses operator< for comparison)
// - O(log n) for insertion, deletion, lookup
// - Good for membership testing and deduplication
//
// Common operations:
// - insert(val): insert element
// - find(val): find element, returns end() if not found
// - count(val): returns 1 if exists, 0 if not
// - erase(val): remove element
// - empty(): check if empty
// - size(): number of elements
//
// Set operations (using std::set_union, std::set_intersection, etc.):
// - Union: all elements from both sets
// - Intersection: elements in both sets
// - Difference: elements in one but not the other
//
// https://www.learncpp.com/cpp-tutorial/stdset/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <set>

// Checks if a set contains a value.
bool contains(const std::set<int> &s, int value)
{
    return s.find(value) != s.end();
}

// Returns the intersection of two sets (elements in both).
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

// Returns the union of two sets (all unique elements).
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
