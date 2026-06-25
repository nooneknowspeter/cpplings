// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// In C++, variable arguments (varargs) can be handled using:
// 1. C-style va_list (from <cstdarg>) - shown in this exercise
// 2. Variadic templates - the modern, type-safe approach (see exercise 10)
//
// Key concepts of va_list:
// - va_list: declares a variable to hold argument list info
// - va_start(args, last_fixed): initializes args after the last fixed parameter
// - va_arg(args, type): retrieves next argument of specified type
// - va_end(args): cleans up the argument list
//
// IMPORTANT: C-style varargs are NOT type-safe!
// - If you pass wrong types, you get undefined behavior
// - No compile-time checking
// - Modern C++ prefers variadic templates (exercise 10)
//
// https://www.learncpp.com/cpp-tutorial/ellipsis-and-why-to-avoid-them/

#include <cstdarg>
#include <cstdio>
#include <gtest/gtest.h>

// Calculates the sum of integers using va_list.
// The first argument specifies how many integers follow.
int sumInts(int count, ...)
{
    va_list args;
    va_start(args, count);

    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += va_arg(args, int);
    }

    va_end(args);
    return sum;
}

// Prints formatted output using va_list.
// Uses printf-style format string with variable arguments.
void printFormatted(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
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
