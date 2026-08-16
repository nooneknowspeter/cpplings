// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// The main() function is an essential part of every C++ project.
// It is the entry point to any and every application/project/piece of software.
//
// The main() function returns an integer, but some of the exercises you noticed
// that some of them don't return any integer, just like a void function
// omitting the "return" keyword. The main function allows this, it implicitly
// handles the returned value.
//
// However, in the previous cases you may have encountered booleans. When
// booleans are printed to stdout, they appear to be different than "true" and
// "false", 1 and 0 respectively. More on booleans in upcoming exercises.
//
// The main() function does not return a boolean value, it just returns an
// integer. A main() function returning 0 returns "0 errors". Any value other
// the 0 means an error. Explicitly returning 0; { return 0; } is considered
// good practice in C++ development.
//
// The parameters in the main(int argc, char *argv[]) function are the
// program's/application's arguments:
//
//             main(int argc, char *argv[])
//                      |           |
//                      |           +-> char *argv[] (also written
//                      |               as char **argv): an array of
//                      |               string slices or string literals
//                      +-> argc: number of arguments passed
//                          into the main function; used for
//                          iterating the argument array
//
// When a compiled program is called, the name of the program or the program
// executable is the first argument. In programs/applications, especially CLI
// programs that use arguments; ./program arg1 arg2, are also passed to the
// main() function.
//
// You can compile this specific exercise using "zig build exercises --
// ./exercises/path/to/exercise/file", ensure you pass in the correct path of
// this file. The binary will be in ./zig-out/bin/cpplings_exercise Run it and
// experiment with the arguments.
//
// We also recommend coming back to this exercise and try out the concepts that
// you will learn in the upcoming exercises.
//
// https://www.learncpp.com/cpp-tutorial/statements-and-the-structure-of-a-program/
// https://www.learncpp.com/cpp-tutorial/command-line-arguments/

#include <iostream>

int main(int argc, char* argv[]) {
  // TODO: Print out the argc and argv.
  std::cout << "Number of arguments: " << argc << "\n"
            << "Arguments: " << *argv << std::endl;
}
