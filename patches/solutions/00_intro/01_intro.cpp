// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out. If you got everything working and feel
// ready for the next exercise, enter `n` in the terminal.

#include <gtest/gtest.h>
#include <iostream>
#include <string>

void intro01()
{
    // TODO: Enter the correct passphrase.
    // NOTE: Use the testcase as reference.
    std::cout << "Welcome to " << "cpplings" << "!" << std::endl;
}

int main(int argc, char *argv[])
{
    intro01();

    std::cout << "\n\n" << "Testing output begins here\n--------------------------" << "\n";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Intro, Intro01)
{
    testing::internal::CaptureStdout();
    intro01();
    std::string stdout{testing::internal::GetCapturedStdout()};
    ASSERT_EQ(stdout, "Welcome to cpplings!\n");
}
