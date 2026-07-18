// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Structs group related data together.
//
// Key concepts:
// - Similar to classes but members are public by default
// - Can have member functions (methods)
// - Can have constructors and destructors
// - Use for plain old data (POD) structures
//
// https://www.learncpp.com/cpp-tutorial/structs/

#include <cmath>
#include <gtest/gtest.h>

// TODO: Create a Point struct with x and y coordinates (both int).
struct Point
{
    int x;
    int y;
};

// TODO: Create a function that calculates the distance between two points.
double distance(const Point &a, const Point &b)
{
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Structs, Structs01)
{
    Point p1 = {0, 0};
    Point p2 = {3, 4};

    ASSERT_EQ(p1.x, 0);
    ASSERT_EQ(p1.y, 0);
    ASSERT_EQ(p2.x, 3);
    ASSERT_EQ(p2.y, 4);

    ASSERT_DOUBLE_EQ(distance(p1, p2), 5.0);
}
