// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::vector is a dynamic array that can grow and shrink.
//
// Key features:
// - Random access (like an array): v[i]
// - Dynamic size (grows as needed)
// - Automatic memory management
// - Most commonly used container in C++
//
// Common operations:
// - push_back(val): add to end
// - pop_back(): remove from end
// - size(): number of elements
// - empty(): check if empty
// - clear(): remove all elements
// - insert(), erase(): insert/remove at position
//
// https://www.learncpp.com/cpp-tutorial/vector-capacity-and-stack-behavior/

#include <gtest/gtest.h>
#include <vector>

// TODO: Implement a function that finds the sum of all elements in a vector.
int sumVector(const std::vector<int> &v)
{
    int sum = 0;
    for (int num : v)
    {
        sum += num;
    }
    return sum;
}

// TODO: Implement a function that finds the maximum value in a vector.
int findMax(const std::vector<int> &v)
{
    int max = v[0];
    for (int num : v)
    {
        if (num > max)
        {
            max = num;
        }
    }
    return max;
}

// TODO: Implement a function that reverses a vector.
std::vector<int> reverseVector(const std::vector<int> &v)
{
    std::vector<int> result;
    for (int i = v.size() - 1; i >= 0; i--)
    {
        result.push_back(v[i]);
    }
    return result;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Containers, Containers01)
{
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    ASSERT_EQ(sumVector(v1), 15);
    ASSERT_EQ(sumVector({}), 0);
    ASSERT_EQ(sumVector({10}), 10);

    std::vector<int> v2 = {3, 7, 2, 9, 4};
    ASSERT_EQ(findMax(v2), 9);

    std::vector<int> v3 = {1, 2, 3};
    std::vector<int> expected = {3, 2, 1};
    ASSERT_EQ(reverseVector(v3), expected);
}
