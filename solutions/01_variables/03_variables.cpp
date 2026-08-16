// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Besides the variable data types, C++ has a few more
// keywords for declaring the type of variable before
// decalaring the variable's data type.
//
// const FOO{5};
//
// Constant names are normally written in UPPER_CASE or CONSTANT_CASE.
//
// https://www.learncpp.com/cpp-tutorial/keywords-and-naming-identifiers/

#include <iostream>
#include <type_traits>

void variables03() {
  // TODO: Declare a constant variable with a value and then print it out to
  // stdout.
  const int FOO{5};

  std::cout << FOO;

  static_assert(
      std::is_const_v<decltype(FOO)>,
      "The variable must be a constant.");  // NOTE: Ignore this line of code.
}

int main() { variables03(); }
