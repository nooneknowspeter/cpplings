// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// https://www.learncpp.com/cpp-tutorial/variadic-functions/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

// Variadic templates are the modern, type-safe alternative to C-style va_list.
//
// PROS OF VARIADIC TEMPLATES:
// - Type-safe: no implicit conversions
// - Works with any type (not just integers)
// - IDE support: autocomplete and type checking
// - No runtime overhead: templates are resolved at compile time
// - Works with references, classes, and custom types
//
// CONS OF VARIADIC TEMPLATES:
// - More complex syntax
// - Slower compile times
// - Cannot directly pass to C-style variadic functions
// - Requires recursive template instantiation
//
// PROS OF C-STYLE va_list:
// - Simple syntax for basic cases
// - Faster compile times
// - Can interoperate with C code
// - Easy to use with printf-style functions
//
// CONS OF C-STYLE va_list:
// - Not type-safe
// - Only works with fundamental types (int, double, etc.)
// - No IDE support or autocomplete
// - Undefined behavior if types don't match

// Base case: single argument - just return it.
template <typename T> T sum(T value)
{
    return value;
}

// TODO: Implement the recursive case for sum.
// The function should add the first argument to the sum of the rest.
// Hint: Use template parameter packs (typename... Args) and recursion.
template <typename T, typename... Args> T sum(T first, Args... args)
{
    // TODO: Return first + sum(args...)
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions10")
{
    REQUIRE(sum(1, 2, 3) == 6);
    REQUIRE(sum(1, 2, 3, 4, 5) == 15);
    REQUIRE(sum(10) == 10);
    REQUIRE(sum(1.5, 2.5, 3.0) == 7.0);
}
