// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::string_view conversion to std::string:
// - std::string has a constructor that accepts string_view
// - Converting string_view to string creates a copy of the data
//
// When to convert:
// - When you need to modify the string (string_view is read-only)
// - When passing to older APIs that require std::string
// - When you need ownership semantics
//
// https://www.learncpp.com/cpp-tutorial/stdstring_view/

#include <gtest/gtest.h>
#include <string>
#include <string_view>

// TODO: Implement a function that converts a string_view to a string.
// If the string_view is empty, return an empty string.
std::string stringViewToString(std::string_view sv)
{
    return std::string(sv);
}

// TODO: Implement a function that trims whitespace from both ends of a string_view.
// Return a new string (not a view) with the trimmed result.
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

// TODO: Implement a function that replaces all occurrences of a character
// in a string_view with another character. Return a new string.
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Strings, Strings04)
{
    ASSERT_EQ(stringViewToString("hello"), "hello");
    ASSERT_EQ(stringViewToString(""), "");

    ASSERT_EQ(trim("  hello  "), "hello");
    ASSERT_EQ(trim("hello"), "hello");
    ASSERT_EQ(trim("   "), "");
    ASSERT_EQ(trim("\t\ntest\t\n"), "test");

    ASSERT_EQ(replaceAll("hello world", 'l', 'L'), "heLLo worLd");
    ASSERT_EQ(replaceAll("aaa", 'a', 'b'), "bbb");
    ASSERT_EQ(replaceAll("no changes", 'x', 'y'), "no changes");
}
