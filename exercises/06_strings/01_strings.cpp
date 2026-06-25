// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C-style strings (C++ inherited from C):
// - Array of chars terminated by '\0' (null terminator)
// - char str[] = "hello";
// - Use <cstring> functions: strlen, strcpy, strcmp
//
// std::string (C++):
// - Dynamic string class from <string>
// - Automatic memory management
// - Many useful methods: length, substr, find, etc.
//
// https://www.learncpp.com/cpp-tutorial/c-style-strings/
// https://www.learncpp.com/cpp-tutorial/stdstring-overview/

#include <cstring>
#include <gtest/gtest.h>
#include <string>

// TODO: Implement a function that returns the length of a C-string.
// Don't use strlen - implement it yourself!
int cstringLength(const char *str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

// TODO: Implement a function that compares two C-strings.
// Return 0 if equal, negative if s1 < s2, positive if s1 > s2.
int cstringCompare(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// TODO: Implement a function that concatenates two C-strings.
// Return a new dynamically allocated string.
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
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Strings, Strings01)
{
    ASSERT_EQ(cstringLength("hello"), 5);
    ASSERT_EQ(cstringLength(""), 0);
    ASSERT_EQ(cstringLength("A"), 1);

    ASSERT_EQ(cstringCompare("apple", "banana"), -1);
    ASSERT_EQ(cstringCompare("hello", "hello"), 0);
    ASSERT_EQ(cstringCompare("zebra", "apple"), 1);

    char *concat = cstringConcat("hello", "world");
    ASSERT_STREQ(concat, "helloworld");
    delete[] concat;
}
