// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// https://www.learncpp.com/cpp-tutorial/recursion/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

// TODO: Implement the recursive fibonacci function.
// NOTE: Use the test case as reference.
fibonacci(int n)
{
    if (n == 0 || n == 1)
    {
        return n;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions07")
{
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    for (int n = 0; n <= 10; ++n)
    {
        if (n == 10)
        {
            std::cout << fibonacci(n) << ",..." << std::endl;
            continue;
        }

        std::cout << fibonacci(n) << ", ";
    }

    std::cout.rdbuf(old);
    std::string output{buffer.str()};

    REQUIRE(output == "0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,...\n");

    std::cout << "\n" << output << "\n";
}
