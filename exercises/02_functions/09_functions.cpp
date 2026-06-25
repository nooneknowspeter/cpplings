// https://www.learncpp.com/cpp-tutorial/ellipsis-and-why-to-avoid-them/

#include <cstdarg>
#include <cstdio>
#include <gtest/gtest.h>

// C-style variable arguments using va_list.
// Note: This approach is NOT type-safe. Prefer variadic templates (exercise 10).

// TODO: Implement sumInts using va_list.
// The first argument (count) specifies how many integers follow.
// Use va_start, va_arg, and va_end macros from <cstdarg>.
int sumInts(int count, ...)
{
    // TODO: Implement this function
    return 0;
}

// TODO (optional): Implement printFormatted using va_list.
// Uses printf-style format string with variable arguments.
void printFormatted(const char *format, ...)
{
    // Hint: Use vprintf instead of printf for va_list
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Functions, Functions09)
{
    ASSERT_EQ(sumInts(3, 1, 2, 3), 6);
    ASSERT_EQ(sumInts(5, 1, 2, 3, 4, 5), 15);
    ASSERT_EQ(sumInts(1, 42), 42);
}
