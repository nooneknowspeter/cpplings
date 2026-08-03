// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// https://www.learncpp.com/cpp-tutorial/ellipsis-and-why-to-avoid-them/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdarg>
#include <cstdio>

// C-style variable arguments using va_list.
// Note: This approach is NOT type-safe. Prefer variadic templates (exercise 10).

// TODO: Implement sumInts using va_list.
// The first argument (count) specifies how many integers follow.
// Use va_start, va_arg, and va_end macros from <cstdarg>.
int sumInts(int count, ...)
{
    // TODO: Implement this function
}

// TODO (optional): Implement printFormatted using va_list.
// Uses printf-style format string with variable arguments.
void printFormatted(const char *format, ...)
{
    // Hint: Use vprintf instead of printf for va_list
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions09")
{
    REQUIRE(sumInts(3, 1, 2, 3) == 6);
    REQUIRE(sumInts(5, 1, 2, 3, 4, 5) == 15);
    REQUIRE(sumInts(1, 42) == 42);
}
