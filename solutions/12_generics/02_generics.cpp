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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

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
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Generics Generics02")
{
    Pair<int, std::string> p(42, "hello");
    REQUIRE(p.getFirst() == 42);
    REQUIRE(p.getSecond() == "hello");

    Pair<double, char> p2(3.14, 'c');
    REQUIRE(p2.getFirst() == Catch::Approx(3.14));
    REQUIRE(p2.getSecond() == 'c');

    Box<int> emptyBox;
    REQUIRE(emptyBox.isEmpty());

    Box<int> fullBox(42);
    REQUIRE_FALSE(fullBox.isEmpty());
    REQUIRE(fullBox.getValue() == 42);
}
