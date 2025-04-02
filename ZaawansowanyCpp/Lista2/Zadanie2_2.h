#pragma once

#include <iostream>


template <int N>
class Factorial {
public:
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
class Factorial<0> {
public:
    static constexpr int value = 1;
};

int main2_2()
{
    Factorial<0> factorial0;
    std::cout << decltype(factorial0)::value << "\n";

    Factorial<1> factorial1;
    std::cout << decltype(factorial1)::value << "\n";

    std::cout << Factorial<2>::value << "\n";
    std::cout << Factorial<3>::value << "\n";
    std::cout << Factorial<4>::value << "\n";
    std::cout << Factorial<5>::value << "\n";


	return 0;
}