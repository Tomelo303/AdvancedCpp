#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


template <typename T>
std::vector<const T*> as_sorted_view(const std::vector<T>& vec)
{
	std::vector<const T*> result = {};
	
	for (auto& v : vec)
		result.push_back(&v);

	std::sort(result.begin(), result.end(), [](const T* a, const T* b)
		{
			return *a < *b;
		});

	return result;
}

int main3_2()
{
	std::vector<std::string> vec = { "zupa", "kura", "jajo", "arbuz", "babilon" };

	auto output = as_sorted_view(vec);

	for (auto& v : output)
		std::cout << *v << "\n";

	std::cout << "\n";

	for (auto& v : vec)
		std::cout << v << "\n";

	return 0;
}