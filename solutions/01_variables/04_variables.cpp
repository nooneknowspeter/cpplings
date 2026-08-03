// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

void variables04()
{
    // TODO: Change the line below to fix the compiler error.
    // NOTE: Use the test case as reference.
    int baz{5};

    // TODO: Also print out the value of baz to stdout in a sentence "baz=5".
    std::cout << "baz=" << baz;
}

int main(int argc, char **argv)
{
    variables04();

    std::cout << "\n\n" << "Testing output begins here\n--------------------------" << "\n";
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Variables Variables04")
{
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    variables04();

    std::cout.rdbuf(old);
    std::string output{buffer.str()};
    REQUIRE(output == "baz=5");
}
