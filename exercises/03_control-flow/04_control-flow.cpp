// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// The for loop is the most common loop in C++.
//
// Syntax:
// for (initialization; condition; increment) {
//     // code
// }
//
// C++ also has a range-based for loop for iterating over containers:
//
// for (element_type element : container) {
//     // code
// }
//
// https://www.learncpp.com/cpp-tutorial/for-statements/
// https://www.learncpp.com/cpp-tutorial/range-based-for-statements/

#include <array>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>

// TODO: Implement a function that calculates the sum of all numbers
// from 1 to n using a for loop.
int sumToN(int n)
{
    int sum = 0;
    for (int i = 1; i = n; i++)
    {
        sum += i;
    }
    return sum;
}

// TODO: Implement a function that finds the maximum value in an array
// using a for loop.
int findMax(const int *arr, int size)
{
    int max = arr[0];
    for (int i = 1; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

// TODO: Implement a function that reverses a string using a for loop.
// Return the reversed string.
std::string reverseString(const std::string &str)
{
    std::string result;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        result += str[i];
    }
    return result;
}

// TODO: Implement a function that calculates the sum of all elements
// in a std::vector using a range-based for loop.
int sumVector(const std::vector<int> &vec)
{
    int sum = 0;
    for (int num : vec)
    {
        sum += num;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("ControlFlow ControlFlow04")
{
    REQUIRE(sumToN(5) == 15);
    REQUIRE(sumToN(10) == 55);

    int arr[] = {3, 7, 2, 9, 4};
    REQUIRE(findMax(arr, 5) == 9);

    REQUIRE(reverseString("hello") == "olleh");
    REQUIRE(reverseString("a") == "a");

    std::vector<int> vec = {1, 2, 3, 4, 5};
    REQUIRE(sumVector(vec) == 15);
}
