// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Class templates create generic classes.
//
// Key concepts:
// - Similar to function templates but for classes
// - All member functions are also templates
// - Often defined entirely in headers
// - Can have multiple template parameters
//
// std::pair<T1, T2> is a classic example:
// - Stores two values of potentially different types
// - Provides first and second accessors
//
// When to use class templates:
// - Need to store multiple values generically
// - Need stateful generic types
// - Implementing generic data structures
//
// https://www.learncpp.com/cpp-tutorial/class-templates/

#include <gtest/gtest.h>

// A simple generic pair class.
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

// A generic box that may or may not contain a value.
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
