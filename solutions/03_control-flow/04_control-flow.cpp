// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// The for loop is the most versatile loop in C++.
//
// Syntax:
// for (init; condition; increment) { body }
//
// - init: runs once before loop starts (usually loop variable)
// - condition: checked before each iteration
// - increment: runs after each iteration
//
// Range-based for loop (C++11):
// for (auto element : container) { body }
//
// Key concepts:
// - for loops are ideal when you know the number of iterations
// - Range-based for is cleaner for iterating containers
// - Always ensure loop termination to avoid infinite loops
//
// https://www.learncpp.com/cpp-tutorial/for-statements/
// https://www.learncpp.com/cpp-tutorial/range-based-for-statements/

#include <array>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

// Calculates sum of integers from 1 to n using a for loop.
int sumToN(int n)
{
    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }
    return sum;
}

// Finds the maximum value in an array.
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

// Reverses a string using a for loop.
std::string reverseString(const std::string &str)
{
    std::string result;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        result += str[i];
    }
    return result;
}

// Sums all elements in a vector using range-based for.
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
