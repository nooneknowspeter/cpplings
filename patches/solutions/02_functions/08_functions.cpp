// Lambdas are anonymous functions.
// In C++, they are not regular functions, under the hood they're called functors.
//
// A C++ lambda includes:
// - The function signature: [] (int x) -> int;
//													 |	|						|
//													 |	|						return type denoted using:
//													 |	|						-> return type
//													 |  |
//													 |	parameter list
//													 |
//													 capture clause;
//													 access data outside the lambda
//													 by "reference" & or by "value"
//
// - The funtion clause/body:
// { -> function scope beginning
// }; -> function scope ending, needs a semi colon (not a native function)
//
// https://www.learncpp.com/cpp-tutorial/introduction-to-lambdas-anonymous-functions/
// https://www.learncpp.com/cpp-tutorial/lambda-captures/

#include <iostream>

int main()
{
    auto y = [&](int x) -> int { return x * 5; };

    std::cout << y(10) << std::endl;
}
