// The function return a value and the value's datatype must be the same
// as the function's return type.
//
// https://www.learncpp.com/cpp-tutorial/function-return-values-value-returning-functions/
// https://www.learncpp.com/cpp-tutorial/introduction-to-function-parameters-and-arguments/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

// TODO: Add the missing keyword.
// NOTE: Use the rest of the code as reference to determine which datatype needs to be returned.
int bar()
{
    // TODO: Change the line below to fix the compiler error.
    // NOTE: Use the test case as reference.
    int baz{5};

    // TODO: Also print out the value of baz to stdout in a sentence "baz=5".
    std::cout << "baz=" << baz;

    return baz;
}

int main(int argc, char **argv)
{
    // TODO: call the bar function
    bar();

    std::cout << "\n\n" << "Testing output begins here\n--------------------------" << "\n";
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Variables Variables04")
{
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    bar();

    std::cout.rdbuf(old);
    std::string output{buffer.str()};
    REQUIRE(bar() == 5);
    REQUIRE(output == "baz=5");
}
