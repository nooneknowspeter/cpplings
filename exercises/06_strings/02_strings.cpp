// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// std::string provides a safer and more feature-rich alternative to C-strings.
//
// Common operations:
// - + : concatenation
// - ==, !=, <, > : comparison
// - length(), size() : get length
// - substr(pos, len) : extract substring
// - find(str) : find substring
// - append(str) : add to end
// - clear() : empty the string
//
// https://www.learncpp.com/cpp-tutorial/stdstring-construction-and-destruction/
// https://www.learncpp.com/cpp-tutorial/stdstring-character-access-and-conversion-to-cstyle/

#include <gtest/gtest.h>
#include <string>

// TODO: Implement a function that reverses a std::string.
std::string reverseString(const std::string &str)
{
    std::string result;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        result += str[i];
    }
    return result;
}

// TODO: Implement a function that counts occurrences of a character.
int countChar(const std::string &str, char c)
{
    int count = 0;
    for (char ch : str)
    {
        if (ch == c)
        {
            count++;
        }
    }
    return count;
}

// TODO: Implement a function that checks if a string is a palindrome.
bool isPalindrome(const std::string &str)
{
    int left = 0;
    int right = str.length() - 1;
    while (left < right)
    {
        if (str[left] != str[right])
        {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Strings, Strings02)
{
    ASSERT_EQ(reverseString("hello"), "olleh");
    ASSERT_EQ(reverseString("a"), "a");
    ASSERT_EQ(reverseString("racecar"), "racecar");

    ASSERT_EQ(countChar("hello", 'l'), 2);
    ASSERT_EQ(countChar("banana", 'a'), 3);
    ASSERT_EQ(countChar("test", 'x'), 0);

    ASSERT_TRUE(isPalindrome("racecar"));
    ASSERT_TRUE(isPalindrome("a"));
    ASSERT_TRUE(isPalindrome("noon"));
    ASSERT_FALSE(isPalindrome("hello"));
}
