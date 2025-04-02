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

		// Move elements of vec that are greater than key to one position ahead of their current position
		while (j >= 0 && vec[j] > key)
		{
			vec[j + 1] = vec[j];

			j -= 1;  // Move one step back
		}

		vec[j + 1] = key;  // Insert the key into the position where no elements are greater than key
	}
}

int char2int(char c)
{
	return c - '0';
}

bool is_digit(char c)
{
	return char2int(c) >= 0 && char2int(c) < 10;
}

bool is_only_digits(const std::string& str)
{
	bool result = true;

	if (str.size() == 0)
	{
		result = false;
	}
	else
	{
		for (auto& c : str)
		{
			if (!is_digit(c))
				result = false;
		}
	}

	return result;
}

std::vector<std::string> separate_numbers(const std::string& str)
{
	std::string temp = "";  // Temporary string for storing the current part of the input string
	std::vector<std::string> result = {};  // Vector of which either item is a number or set of any characters besides numbers
	bool was_digit = is_digit(str[0]);  // True if the previous character was a digit

	temp += str[0];

	for (int i = 1; i < str.size(); i++)
	{
		if (was_digit != is_digit(str[i]))
		{
			result.push_back(temp);
			temp = str[i];
			was_digit = is_digit(str[i]);
		}
		else
		{
			temp += str[i];
		}
	}
	
	result.push_back(temp);

	return result;
}

std::string concatenate(const std::vector<std::string>& vec)
{
	std::string result = "";

	for (auto& str : vec)
		result += str;

	return result;
}

template <>
void inline insertion_sort(std::vector<std::string>& vec)
{
	std::vector<std::vector<std::string>> separated = {};  // Vector of strings separated by numbers

	// Separate numbers in the passed strings
	for (int i = 0; i < vec.size(); i++)
		separated.push_back(separate_numbers(vec[i]));

	// Calculate the size of the shortest sub-vector in the separated vector
	int min_size = separated[0].size();
	for (int i = 1; i < separated.size(); i++)
	{
		if (min_size > separated[i].size())
			min_size = separated[i].size();
	}
	//std::cout << "\nmin_size = " << min_size << "\n\n";

	// Iterate through a sub-string backwards
	for (int k = min_size - 1; k >= 0; k--)
	{
		//std::cout << "k = " << k << "\n\n";

		// Sort
		for (int i = 1; i < vec.size(); i++)
		{
			auto key = separated[i];  // The current element to be inserted
			int j = i - 1;

			//std::cout << "\x1b[36mFOR\x1b[0m loop:   \x1b[33m" << separated << "\x1b[0m; i = " << i << "; j = " << j << "; key = \x1b[94m" << key << "\x1b[0m\n\n";

			if (is_only_digits(separated[j][k]))  // Sort naturally
			{
				// Move elements of vec that are greater than key to one position ahead of their current position
				while (j >= 0 && std::stoi(separated[j][k]) > std::stoi(key[k]))
				{
					//std::cout << "\x1b[32mSORTING NATURALLY\x1b[0m\n";
					//std::cout << "\x1b[31mWHILE\x1b[0m loop: " << "vec[" << j + 1 << "] = " << separated[j];
					
					separated[j + 1] = separated[j];
					
					//std::cout << " --> " << separated << "\n";
					
					j -= 1;  // Move one step back
				}
			}
			else  // Sort lexicographically
			{
				// Move elements of vec that are greater than key to one position ahead of their current position
				while (j >= 0 && separated[j][k] > key[k])
				{
					//std::cout << "\x1b[35mSORTING LEXICOGRAPHICALLY\x1b[0m\n";
					//std::cout << "\x1b[31mWHILE\x1b[0m loop: " << "vec[" << j + 1 << "] = " << separated[j];
					
					separated[j + 1] = separated[j];
					
					//std::cout << " --> " << separated << "\n";
					
					j -= 1;  // Move one step back
				}
			}

			separated[j + 1] = key;  // Insert the key into the position where no elements are greater than key

			//std::cout << "\x1b[94mInsert\x1b[0m vec[" << j + 1 << "] = \x1b[94m" << key << "\x1b[0m\n\n";
		}

		//std::cout << "\x1b[90mENDFOR\x1b[0m\n\n----> \x1b[43m\x1b[30m" << separated << "\x1b[0m\n\n\n";
	}

	// Join the sorted parts of strings together and insert them into the passed vector
	for (int i = 0; i < vec.size(); i++)
		vec[i] = concatenate(separated[i]);
}

int main2_1()
{
	std::vector<int> vec0 = { 4, 2, 6, 10, 5, 3, 5, 8 };
	insertion_sort(vec0);
	std::cout << "vec0: " << vec0 << "\n\n";

	std::vector<std::string> vec1 = { "Magda88", "Basia13", "Zuzia123", "Zuzia5", "Asia0", "Basia2", "Magda2", "Asia10", "Magda9"};
	insertion_sort(vec1);
	std::cout << "vec1: " << vec1 << "\n";

	std::vector<std::string> vec2 = {"3 Rok 5 Semestr 3 Lista", "2 Rok 3 Semestr 12 Lista", "2 Rok 4 Semestr 2 Lista", "3 Rok 5 Semestr 10 Lista", "2 Rok 4 Semestr 1 Lista", "2 Rok 3 Semestr 2 Lista"};
	insertion_sort(vec2);
	std::cout << "vec2: " << vec2 << "\n";

	std::vector<std::string> vec3 = { "2002.11.11", "1998.05.12", "1998.05.02", "2002.02.20" };
	insertion_sort(vec3);
	std::cout << "vec3: " << vec3 << "\n";


	return 0;
}
