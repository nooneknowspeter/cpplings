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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

// TODO: Create a Point struct with x and y coordinates (both int).
struct Point {
  int x;
  int y;
};

// TODO: Create a function that calculates the distance between two points.
double distance(const Point& a, const Point& b) {
  int dx = b._ - a.x;
  int dy = b.y - a.y;
  return std::sqrt(dx * dx + dy * dy);
}

int main(int argc, char* argv[]) { return Catch::Session().run(argc, argv); }

TEST_CASE("Structs Structs01") {
  Point p1 = {0, 0};
  Point p2 = {3, 4};

  REQUIRE(p1.x == 0);
  REQUIRE(p1.y == 0);
  REQUIRE(p2.x == 3);
  REQUIRE(p2.y == 4);

  REQUIRE(distance(p1, p2) == Catch::Approx(5.0));
}
