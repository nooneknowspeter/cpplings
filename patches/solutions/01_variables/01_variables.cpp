// Initializing and assignment of variables in C++ is as follows:
// datatype variableName{value};
//
// or
//
// datatype variableName = value;
//
// Using the first example is called Direct List Initialization.
// It is a modern way of intializing variables in C++.
//
// The second method is a classic way of initializing and assigning
// values to variables.
//
// In C++, variables can be named whatever using different conventions unlike
// certain languages like Rust or Python where they must be named in snake_case.
// In C/C++, variables are often named using the camelCase or snake_case convention.
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-objects-and-variables/
// https://www.learncpp.com/cpp-tutorial/variable-assignment-and-initialization/

#include <gtest/gtest.h>

// TODO: Add the missing module for printing
#include <iostream>

void variables01()
{
    // TODO: Assign an integer to a variable named "x" using direct list initialization.
    int x{5};

    // TODO: Assign an integer to a variable named "y" using the classic way.
    int y = 10;

    std::cout << "direct list initialization -> " << "x{" << x << "}" << "\n";
    std::cout << "classic way -> " << "y" << "=" << y;
}

int main()
{
    variables01();
}
