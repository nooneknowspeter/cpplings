// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C-style arrays vs C++ std::array:
//
// C-style array: int arr[5] = {1, 2, 3, 4, 5};
// - Fixed size, no bounds checking
// - Decays to pointer when passed to functions
// - Memory managed manually
//
// std::array: std::array<int, 5> arr = {1, 2, 3, 4, 5};
// - Fixed size (known at compile time)
// - Does NOT decay to pointer
// - Provides member functions (size, empty, at)
// - Safer with bounds-checked .at() method
//
// https://www.learncpp.com/cpp-tutorial/stdarray/

#include <array>
#include <cstddef>
#include <gtest/gtest.h>

// TODO: Implement a function that sums all elements in a std::array.
int sumArray(const std::array<int, 5> &arr)
{
    int sum = 0;
    for (int num : arr)
    {
        sum += num;
    }
    return sum;
}

// TODO: Implement a function that finds an element in an array.
// Return its index, or -1 if not found.
int findInArray(const std::array<int, 5> &arr, int target)
{
    for (int i = 0; i < 5; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

// TODO: Implement a function that doubles each element in an array.
std::array<int, 5> doubleArray(const std::array<int, 5> &arr)
{
    std::array<int, 5> result;
    for (int i = 0; i < 5; i++)
    {
        result[i] = arr[i] * 2;
    }
    return result;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Containers, Containers02)
{
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    ASSERT_EQ(sumArray(arr), 15);

    ASSERT_EQ(findInArray(arr, 3), 2);
    ASSERT_EQ(findInArray(arr, 6), -1);

    std::array<int, 5> doubled = {2, 4, 6, 8, 10};
    ASSERT_EQ(doubleArray(arr), doubled);
}
