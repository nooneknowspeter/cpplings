//
//
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-functions/
// https://www.learncpp.com/cpp-tutorial/function-return-values-value-returning-functions/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

int function01(int x)
{
    int y{x + 1};

    return y;
}

TEST_CASE("Functions Function01")
{
    REQUIRE(function01(0) == 1);
    REQUIRE(function01(1) == 2);
    REQUIRE(function01(2) == 3);
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}
