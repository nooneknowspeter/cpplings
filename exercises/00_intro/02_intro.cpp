// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Printing in C++ involves "cout", a function from the "std" namespace,
// that displays an stream buffer in the console.
//
// Referencing a function from a namespace, or in this a case a namespace
// from a module (imported using "#include <iostream>"),
// involves using the scope resolution operator (::),
// we will get deeper into modules later.
//
// The value to be printed must "shift to the stream" using the
// left-shift operator (<<).
//
// https://cppbyexample.com/what_is_iostream.html
// http://www.en.cppreference.com/w/cpp/io/cout.html

#include <iostream>

void message()
{
    // TODO: Fix the line of code to print "Hello World!" to the console
    std::cout << "Hello World!";
}
