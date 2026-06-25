// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// C++ is a statically typed language.
// Datatypes need to be explicitly decalred.
// Datatypes can be "inferred" by using the "auto" keyword.

#include <iostream>
int main()
{
    // TODO: Infer the datatype using the "auto" keyword.
    // Assign a "truthy" value to compile.
    auto doesAutoWork{true};

    std::cout << "doesAutoWork? -> " << doesAutoWork;

    if (!doesAutoWork)
    {
        return 1;
    }
}
