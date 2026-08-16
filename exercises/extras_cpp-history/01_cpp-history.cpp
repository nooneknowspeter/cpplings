// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ has changed over the years; many features and fixes.
// Below are references to learn the changes made to C++ from learncpp.com.
//
// [C++ 11](https://www.learncpp.com/cpp-tutorial/introduction-to-c11/)
// [C++ 14](https://www.learncpp.com/cpp-tutorial/introduction-to-c14/)
// [C++ 17](https://www.learncpp.com/cpp-tutorial/introduction-to-c17/)
// [C++ 20](https://www.learncpp.com/cpp-tutorial/introduction-to-c20/)
// [C++ 23](https://www.learncpp.com/cpp-tutorial/introduction-to-c23/)

#include <exception>
#include <iostream>
#include <stdexcept>

int main() {
  // TODO: Make this variable "truthy" to compile.
  // NOTE: Please read the comments above.
  bool hasRead{false};

  try {
    if (!hasRead) {
      throw runtime_error(
          "Please view the code of this exercise before proceeding.");
    }
  } catch (std::exception& e) {
    std::cout << e.what();
  }
}
