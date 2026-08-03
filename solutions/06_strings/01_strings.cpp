// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C-style strings are arrays of characters terminated by '\0'.
//
// Key concepts:
// - char str[] = "hello"; creates: ['h','e','l','l','o','\0']
// - The null terminator marks the end of the string
// - Functions from <cstring> operate on these strings
// - Care must be taken to avoid buffer overflows
//
// Common <cstring> functions:
// - strlen(s): length of string
// - strcpy(dest, src): copy string
// - strcmp(s1, s2): compare strings
// - strcat(dest, src): concatenate strings
//
// Why C-strings matter:
// - Still used in C APIs and low-level programming
// - Understanding them helps with debugging
// - Some performance-critical code uses them
//
// https://www.learncpp.com/cpp-tutorial/c-style-strings/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <string>

// Returns the length of a C-string (manual implementation).
int cstringLength(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// Compares two C-strings lexicographically.
// Returns: 0 if equal, negative if s1 < s2, positive if s1 > s2.
int cstringCompare(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    if (*s1 == *s2)
    {
        return 0;
    }
    return *s1 < *s2 ? -1 : 1;
}

// Concatenates two C-strings and returns a new dynamically allocated string.
char *cstringConcat(const char *s1, const char *s2)
{
    int len1 = cstringLength(s1);
    int len2 = cstringLength(s2);
    char *result = new char[len1 + len2 + 1];

    int i = 0;
    for (; i < len1; i++)
    {
        result[i] = s1[i];
    }
    for (int j = 0; j < len2; j++)
    {
        result[i + j] = s2[j];
    }
    result[len1 + len2] = '\0';

    return result;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Strings Strings01")
{
    REQUIRE(cstringLength("hello") == 5);
    REQUIRE(cstringLength("") == 0);
    REQUIRE(cstringLength("A") == 1);

    REQUIRE(cstringCompare("apple", "banana") == -1);
    REQUIRE(cstringCompare("hello", "hello") == 0);
    REQUIRE(cstringCompare("zebra", "apple") == 1);

    char *concat = cstringConcat("hello", "world");
    REQUIRE(std::string(concat) == "helloworld");
    delete[] concat;
}
