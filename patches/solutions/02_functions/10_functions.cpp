// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Variadic templates are the modern, type-safe alternative to C-style va_list.
// They use '...' syntax to accept any number of arguments of any type.
//
// Key concepts:
// - typename... Args: template parameter pack (holds types)
// - Args... args: function parameter pack (holds values)
// - sizeof...(args): returns number of arguments in the pack
// - Recursion: base case handles single arg, recursive case handles rest
//
// PROS OF VARIADIC TEMPLATES:
// - Type-safe: no implicit conversions
// - Works with any type (int, double, std::string, custom types)
// - IDE support: autocomplete and type checking
// - No runtime overhead: templates resolved at compile time
// - Works with references, classes, and custom types
//
// CONS OF VARIADIC TEMPLATES:
// - More complex syntax than simple functions
// - Slower compile times due to template instantiation
// - Cannot directly pass to C-style variadic functions
// - Requires recursive template instantiation
//
// COMPARISON WITH C-STYLE va_list (exercise 09):
// - va_list: simple syntax, not type-safe, limited types
// - Variadic templates: complex syntax, fully type-safe, any type
//
// https://www.learncpp.com/cpp-tutorial/variadic-functions/

#include <gtest/gtest.h>

// Base case: single argument - just return it
// This terminates the recursion.
template <typename T> T sum(T value)
{
    return value;
}

// Recursive case: add first argument to sum of rest
// typename... Args is a template parameter pack (holds types)
// Args... args is a function parameter pack (holds values)
template <typename T, typename... Args> T sum(T first, Args... args)
{
    // Recursively call sum with remaining arguments
    return first + sum(args...);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Functions, Functions10)
{
    ASSERT_EQ(sum(1, 2, 3), 6);
    ASSERT_EQ(sum(1, 2, 3, 4, 5), 15);
    ASSERT_EQ(sum(10), 10);
    ASSERT_EQ(sum(1.5, 2.5, 3.0), 7.0);
}
