#pragma once

#include <iostream>


template <typename T, typename... Args>
void print_all(T first, Args... args)
{
	if constexpr (sizeof...(args) == 0)
	{
		std::cout << first;
	}
	else
	{
		std::cout << first << ", ";
		print_all(args...);  // Each time the parameter pack gets smaller, because it has to fill the first argument
	}
}


int main2_3()
{
	print_all(false, 1, 2.0, "3"); std::cout << "\n";
	print_all(1, 1.0, 1.0f, "Hello"); std::cout << "\n";
	print_all(1, 2.0); std::cout << "\n";
	print_all(1.0); std::cout << "\n";
	//print_all();  // no instance of function template "print_all" matches the argument list


	return 0;
}