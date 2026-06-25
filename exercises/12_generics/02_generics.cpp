// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Class templates allow creating generic classes that work with any type.
//
// Syntax:
// template <typename T>
// class ClassName { ... };
//
// Template classes are typically defined in headers.
// Use std::pair as an example of a class template.
//
// https://www.learncpp.com/cpp-tutorial/class-templates/

#include <gtest/gtest.h>

// TODO: Implement a simple Pair class template with getFirst() and getSecond().
template <typename T, typename U> class Pair
{
  public:
    T first;
    U second;

    Pair(T f, U s) : first(f), second(s)
    {
    }

    T getFirst() const
    {
        return first;
    }
    U getSecond() const
    {
        return second;
    }
};

// TODO: Implement a Box class template that holds a single value.
// Include a method to get the value and check if it's empty.
template <typename T> class Box
{
    T value;
    bool hasValue;

  public:
    Box() : hasValue(false)
    {
    }
    Box(T v) : value(v), hasValue(true)
    {
    }

    bool isEmpty() const
    {
        return !hasValue;
    }
    T getValue() const
    {
        return value;
    }
};

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Generics, Generics02)
{
    Pair<int, std::string> p(42, "hello");
    ASSERT_EQ(p.getFirst(), 42);
    ASSERT_EQ(p.getSecond(), "hello");

    Pair<double, char> p2(3.14, 'c');
    ASSERT_DOUBLE_EQ(p2.getFirst(), 3.14);
    ASSERT_EQ(p2.getSecond(), 'c');

    Box<int> emptyBox;
    ASSERT_TRUE(emptyBox.isEmpty());

    Box<int> fullBox(42);
    ASSERT_FALSE(fullBox.isEmpty());
    ASSERT_EQ(fullBox.getValue(), 42);
}
