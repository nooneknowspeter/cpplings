// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// https://www.learncpp.com/cpp-tutorial/recursion/

#include "gtest/gtest.h"
#include <iostream>
#include <string>

// TODO: Implement the recursive fibonacci function.
// NOTE: Use the test case as reference.
int fibonacci(int n)
{
    if (n == 0 || n == 1)
    {
        return n;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Functions, Functions07)
{
    testing::internal::CaptureStdout();

    for (int n = 0; n <= 10; ++n)
    {
        if (n == 10)
        {
            std::cout << fibonacci(n) << ",..." << std::endl;
            continue;
        }

        std::cout << fibonacci(n) << ", ";
    }

    std::string stdout{testing::internal::GetCapturedStdout()};

    ASSERT_EQ(stdout, "0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,...\n");

    std::cout << "\n" << stdout << "\n";
}
