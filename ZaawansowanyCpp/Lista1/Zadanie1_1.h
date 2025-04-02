#pragma once

#include <iostream>
#include <string>


template <typename T, typename C>
T inline my_max(T a, T b, C comp)
{
	return comp(a, b);
}

int main1_1()
{
	auto comp = [](auto a, auto b) { return a < b ? b : a; };

	int x = my_max(2, 10, comp);
	std::cout << x << "\n";

	double y = my_max(2.5, 1.0, comp);
	std::cout << y << "\n";

	std::string z = my_max("apricot", "apple", comp);
	std::cout << z << "\n";

	return 0;
}
