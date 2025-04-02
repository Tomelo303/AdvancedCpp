#pragma once

#include <iostream>
#include <vector>
#include <string>


template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
	if (vec.size() > 0)
	{
		out << "[" << vec[0];
		for (int i = 1; i < vec.size(); i++)
		{
			out << ", " << vec[i];
		}
		out << "]";
	}
	else
	{
		out << "[]";
	}

	return out;
}

template <typename T>
void inline insertion_sort(std::vector<T>& vec)
{
	for (int i = 1; i < vec.size(); i++)
	{
		auto key = vec[i];  // The current element to be inserted
		int j = i - 1;

		// Some prints to help understand what's going on
		//std::cout << "FOR loop:   " << vec << "; i = " << i << "; j = " << j << "; key = " << key << "\n\n";
		
		// Move elements of vec that are greater than key to one position ahead of their current position
		while (j >= 0 && vec[j] > key)
		{
			//std::cout << "WHILE loop: " << "vec["<<j+1<<"] = " << vec[j];
			vec[j + 1] = vec[j];
			//std::cout << " --> " << vec << "\n";
			j -= 1;  // Move one step back
		}

		vec[j + 1] = key;  // Insert the key into the position where no elements are greater than key

		//std::cout << "vec["<<j+1<<"] = " << key << "\n";
	}
}


int main1_2()
{
	std::vector<int> vec0 = { 2, 8, 1, 6, 13, 7, 10, 3 };
	insertion_sort<int>(vec0);
	std::cout << "vec0: " << vec0 << "\n";

	std::vector<double> vec1 = { 8.3, 2.5, 10.1, 0.2, 2.1, 3.7, 9.0 };
	insertion_sort<double>(vec1);
	std::cout << "vec1: " << vec1 << "\n";

	std::vector<std::string> vec2 = { "f", "z", "a", "w" };
	insertion_sort(vec2);
	std::cout << "vec2: " << vec2 << "\n";

	std::cout << "\nZlozonosc algorytmu insertion sort wynosi n^2, "
		<< "gdyz posiada on dwie petle iterojace po wszystkich elementach wektora.\n"
		<< "Dla kazdego z elementow, druga petla moze potencjalnie przeiterowac po calym wektorze.\n"
		<< "Ilosc wykonywanych porownan przypomina szereg arytmetyczny, ktorego suma wynosi n(n+1).\n";

	return 0;
}
