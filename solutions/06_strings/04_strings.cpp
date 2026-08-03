// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Converting between string_view and string:
//
// string_view -> string:
// - std::string has a constructor accepting string_view
// - Creates a copy of the data
// - Necessary when you need ownership or to modify
//
// When to convert:
// - When passing to APIs that require std::string
// - When you need to modify the string
// - When you need to take ownership
//
// Trimming and manipulating strings:
// - string_view makes trimming easy (no copying)
// - Remove leading/trailing whitespace
// - Replace characters in strings
//
// https://www.learncpp.com/cpp-tutorial/stdstring_view/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cctype>
#include <string>
#include <string_view>

// Converts a string_view to std::string (creates a copy).
std::string stringViewToString(std::string_view sv)
{
    return std::string(sv);
}

// Trims whitespace from both ends of a string_view.
// Returns a new string with trimmed content.
std::string trim(std::string_view sv)
{
    size_t start = 0;
    size_t end = sv.length();

    while (start < end && std::isspace(sv[start]))
    {
        start++;
    }

    while (end > start && std::isspace(sv[end - 1]))
    {
        end--;
    }

    return std::string(sv.substr(start, end - start));
}

// Replaces all occurrences of a character with another character.
// Returns a new string.
std::string replaceAll(std::string_view sv, char oldChar, char newChar)
{
    std::string result(sv);
    for (char &c : result)
    {
        if (c == oldChar)
        {
            c = newChar;
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Strings Strings04")
{
    REQUIRE(stringViewToString("hello") == "hello");
    REQUIRE(stringViewToString("") == "");

    REQUIRE(trim("  hello  ") == "hello");
    REQUIRE(trim("hello") == "hello");
    REQUIRE(trim("   ") == "");
    REQUIRE(trim("\t\ntest\t\n") == "test");

    REQUIRE(replaceAll("hello world", 'l', 'L') == "heLLo worLd");
    REQUIRE(replaceAll("aaa", 'a', 'b') == "bbb");
    REQUIRE(replaceAll("no changes", 'x', 'y') == "no changes");
}
