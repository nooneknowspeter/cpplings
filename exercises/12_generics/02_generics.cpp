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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>

// TODO: Implement a simple Pair class template with getFirst() and getSecond().
template <typename T, typename U> class Pair
{
  public:
    T first;
    U second;

    Pair(T f, U s) : first(f), second(s)
    {
    }

    getFirst() const
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
