// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::string_view (C++17):
// - A non-owning view into a string
// - Can refer to a std::string OR a C-string
// - No copying involved - just a pointer and length
// - Useful for avoiding unnecessary string copies
//
// When to use string_view:
// - Function parameters that only read a string
// - Parsing strings without copying
// - APIs that accept either C++ or C strings
//
// Caveats:
// - Does NOT own the data - original string must outlive the view!
// - Can lead to dangling pointers if not careful
//
// https://www.learncpp.com/cpp-tutorial/stdstring_view/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

// TODO: Implement a function that returns the first word of a string_view.
// A word is separated by whitespace.
std::string_view firstWord(std::string_view sv)
{
    size_t pos = sv.find(' ');
    if (pos == std::string_view::npos)
    {
        return sv;
    }
    return sv.substr(0, pos);
}

// TODO: Implement a function that checks if a string_view starts with a prefix.
bool startsWith(std::string_view sv, std::string_view prefix)
{
    return sv.substr(0, prefix.length()) == prefix;
}

// TODO: Implement a function that counts words in a string_view.
int countWords(std::string_view sv)
{
    int count = 0;
    bool inWord = false;

    for (char c : sv)
    {
        if (c == ' ' || c == '\t' || c == '\n')
        {
            if (inWord)
            {
                count++;
                inWord = false;
            }
        }
        else
        {
            inWord = true;
        }
    }

    if (inWord)
    {
        count++;
    }

    return count;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Strings Strings03")
{
    REQUIRE(firstWord("hello world") == "hello");
    REQUIRE(firstWord("single") == "single");
    REQUIRE(firstWord("") == "");

    REQUIRE(startsWith("hello", "he"));
    REQUIRE(startsWith("hello", "hello"));
    REQUIRE_FALSE(startsWith("hello", "world"));
    REQUIRE_FALSE(startsWith("", "anything"));

    REQUIRE(countWords("hello world") == 2);
    REQUIRE(countWords("one two three four") == 4);
    REQUIRE(countWords("single") == 1);
    REQUIRE(countWords("") == 0);
}
