#include <gtest/gtest.h>
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Variables, Variables04)
{
    testing::internal::CaptureStdout();
    variables04();
    std::string stdout{testing::internal::GetCapturedStdout()};
    ASSERT_EQ(stdout, "baz=5");
}
