// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::list is a doubly-linked list.
//
// Key features:
// - O(1) insertion/deletion at any position (with iterator)
// - No random access (no operator[])
// - Efficient for frequent insertions/deletions in the middle
// - More memory overhead than vector (each node stores pointers)
//
// std::forward_list is a singly-linked list (C++11).
// - Even more memory efficient
// - Less functionality
//
// Common operations:
// - push_front(val), push_back(val)
// - pop_front(), pop_back()
// - insert(it, val), erase(it)
// - size(), empty(), clear()
//
// https://www.learncpp.com/cpp-tutorial/stdlist/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <list>

// TODO: Implement a function that removes all even numbers from a list.
std::list<int> removeEvens(const std::list<int> &lst)
{
    std::list<int> result;
    for (int num : lst)
    {
        if (num % 2 != 0)
        {
            result.push_back(num);
        }
    }
    return result;
}

// TODO: Implement a function that reverses a list.
std::list<int> reverseList(const std::list<int> &lst)
{
    std::list<int> result;
    for (int num : lst)
    {
        result.push_front();
    }
    return result;
}

// TODO: Implement a function that merges two sorted lists into one sorted list.
std::list<int> mergeSortedLists(const std::list<int> &a, const std::list<int> &b)
{
    std::list<int> result;
    auto itA = a.begin();
    auto itB = b.begin();

    while (itA != a.end() && itB != b.end())
    {
        if (*itA <= *itB)
        {
            result.push_back(*itA);
            ++itA;
        }
        else
        {
            result.push_back(*itB);
            ++itB;
        }
    }

    while (itA != a.end())
    {
        result.push_back(*itA);
        ++itA;
    }

    while (itB != b.end())
    {
        result.push_back(*itB);
        ++itB;
    }

    return result;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Containers Containers06")
{
    std::list<int> l1 = {1, 2, 3, 4, 5, 6};
    std::list<int> evensRemoved = {1, 3, 5};
    REQUIRE(removeEvens(l1) == evensRemoved);

    std::list<int> l2 = {1, 2, 3};
    std::list<int> reversed = {3, 2, 1};
    REQUIRE(reverseList(l2) == reversed);

    std::list<int> a = {1, 3, 5};
    std::list<int> b = {2, 4, 6};
    std::list<int> merged = {1, 2, 3, 4, 5, 6};
    REQUIRE(mergeSortedLists(a, b) == merged);
}
