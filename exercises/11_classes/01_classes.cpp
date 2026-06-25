// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Classes are the foundation of object-oriented programming in C++.
//
// Key concepts:
// - Members are private by default
// - Public interface exposes functionality
// - Private members hide implementation details
// - Constructors initialize objects
// - Member functions (methods) operate on the object's data
//
// Access specifiers:
// - public: accessible everywhere
// - private: accessible only by member functions
// - protected: accessible by member functions and derived classes
//
// https://www.learncpp.com/cpp-tutorial/classes-and-class-members/

#include <gtest/gtest.h>

// TODO: Create a Counter class with:
// - A private int member 'count'
// - A public constructor that initializes count to 0
// - A public increment() method
// - A public getCount() const method
class Counter
{
    int count;

  public:
    Counter() : count(0)
    {
    }

    void increment()
    {
        count++;
    }

    int getCount() const
    {
        return count;
    }
};

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Classes, Classes01)
{
    Counter c;
    ASSERT_EQ(c.getCount(), 0);

    c.increment();
    ASSERT_EQ(c.getCount(), 1);

    c.increment();
    c.increment();
    ASSERT_EQ(c.getCount(), 3);
}
