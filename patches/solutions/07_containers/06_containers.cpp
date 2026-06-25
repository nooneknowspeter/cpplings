// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::list is a doubly-linked list.
//
// Key characteristics:
// - O(1) insertion/deletion at any position (with iterator)
// - No random access (no operator[])
// - More memory overhead than vector (each node has two pointers)
// - Efficient for frequent insertions/deletions in the middle
// - Good for implementing deques, queues, etc.
//
// Common operations:
// - push_front(val), push_back(val): add elements
// - pop_front(), pop_back(): remove elements
// - insert(it, val): insert before position
// - erase(it): remove at position
// - size(): number of elements
// - empty(): true if empty
// - clear(): remove all
// - remove(val): remove all elements equal to val
// - unique(): remove consecutive duplicates
// - sort(), merge(), reverse()
//
// When to use list vs vector:
// - List: frequent insertions/deletions in the middle
// - Vector: random access, cache locality, or append-only
//
// https://www.learncpp.com/cpp-tutorial/stdlist/

#include <gtest/gtest.h>
#include <list>

// Removes all even numbers from a list.
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

// Reverses a list.
std::list<int> reverseList(const std::list<int> &lst)
{
    std::list<int> result;
    for (int num : lst)
    {
        result.push_front(num);
    }
    return result;
}

// Merges two sorted lists into one sorted list.
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Containers, Containers06)
{
    std::list<int> l1 = {1, 2, 3, 4, 5, 6};
    std::list<int> evensRemoved = {1, 3, 5};
    ASSERT_EQ(removeEvens(l1), evensRemoved);

    std::list<int> l2 = {1, 2, 3};
    std::list<int> reversed = {3, 2, 1};
    ASSERT_EQ(reverseList(l2), reversed);

    std::list<int> a = {1, 3, 5};
    std::list<int> b = {2, 4, 6};
    std::list<int> merged = {1, 2, 3, 4, 5, 6};
    ASSERT_EQ(mergeSortedLists(a, b), merged);
}
