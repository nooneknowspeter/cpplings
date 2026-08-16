// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Functions that return nothing are called "void" functions.
// Void is a datatype that means nothing or "null".
// The function can still contain the "return" keyword.
// void someRandomVoidFunction()
// {
//   float bar{5.0f};
//
//   std::cout << bar;
//
//   return;
// }
//
// https://www.learncpp.com/cpp-tutorial/void-functions-non-value-returning-functions/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// TODO: Add the function datatype and name.
// Add two parameters, one that takes in a boolean and the other a float.
voidFunction(bool isTruthy, float randomValue) {
  try {
    if (typeid(isTruthy).name() != typeid(bool).name()) {
      throw std::runtime_error("The first parameter is not a boolean.\n");
    }

    if (typeid(randomValue).name() != typeid(float).name()) {
      throw std::runtime_error("The second parameter is not a float.\n");
    }
  } catch (const std::exception& e) {
    std::cout << e.what();
  }

  // TODO: Implement the additional required logic.
  // NOTE: Use the test case as reference.
  std::cout << "This is a void function!";
}

int main(int argc, char** argv) {
  // TODO: Call the function and pass in the approriate arguments.
  voidFunction(true, 5);

  std::cout << "\n\n"
            << "Testing output begins here\n--------------------------" << "\n";
  return Catch::Session().run(argc, argv);
}

TEST_CASE("Functions Functions03") {
  std::stringstream buffer;
  std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

  // TODO: Call the function and pass in the approriate arguments.
  voidFunction(true, 5);

  std::cout.rdbuf(old);
  std::string output{buffer.str()};
  REQUIRE(output == "This is a void function!");
}
