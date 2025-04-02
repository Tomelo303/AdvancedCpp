#pragma once

#include <iostream>
#include <vector>
#include <map>


template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> vec)
{
	if (!vec.empty())
	{
		out << "[" << vec[0];
		for (int i = 1; i < vec.size(); i++)
			out << ", " << vec[i];
		out << "]";
	}
	else
	{
		out << "[]";
	}

	return out;
}

/// <summary>Class used to solve a problem of choosing the most valuable treasures without exceeding the weight limit.</summary>
class Treasury
{
  public:
	/// <summary>Default constructor.</summary>
	Treasury() = default;

	/// <summary>Constructor that finds the indices of treasures and the biggest total value from taking them without exceeding weight limit.</summary>
	/// <param name="values">- worth of each treasure</param>
	/// <param name="weights">- weight of each treasure</param>
	/// <param name="weight_limit">- value that limits the number of treasures that can be taken</param>
	Treasury(std::vector<unsigned> values, std::vector<unsigned> weights, unsigned weight_limit) { operator()(values, weights, weight_limit); }


	/// <summary>Finds the indices of treasures and the biggest total value from taking them without exceeding weight limit.</summary>
	/// <param name="values">- worth of each treasure</param>
	/// <param name="weights">- weight of each treasure</param>
	/// <param name="weight_limit">- value that limits the number of treasures that can be taken</param>
	/// <returns>Vector of indices of treasures being the optimal solution</returns>
	std::vector<unsigned> operator()(std::vector<unsigned> values, std::vector<unsigned> weights, unsigned weight_limit)	
	{
		if (values.size() != weights.size()) throw std::invalid_argument("Vectors must be the same size.");
		if (values.empty()) throw std::invalid_argument("Vectors must not be empty.");

		memory.clear();

		_values = values;
		_weights = weights;

		return _solution = optimal_solution(values.size(), weight_limit);
	}

	/// <returns>Vector of indices of treasures being the optimal solution</returns>
	std::vector<unsigned> solution() const { return _solution; }

	/// <returns>The value gained from taking treasures from the solution</returns>
	unsigned value() const { return _value; }

  private:
	unsigned _value = 0;
	std::vector<unsigned> _solution;

	std::vector<unsigned> _values;
	std::vector<unsigned> _weights;
	
	std::map<std::pair<unsigned, unsigned>, std::vector<unsigned>> memory;


	// DO NOT INCLUDE
	/// <summary>
	/// Calculates the maximum value achievable for the given parameters.
	/// Makes use of memoization by populating memory map.</summary>
	/// <param name="n">- number of treasures left</param>
	/// <param name="w">- weight capacity left</param>
	//unsigned optimal_value(unsigned n, unsigned w)
	//{
	//	if (n == 0 || w == 0)
	//		return 0;

	//	unsigned idx = n - 1;  // Index of the current treasure
	//	unsigned result = 0;
	//	auto it = memory.find({ n, w });

	//	if (it == memory.end())  // Pair {n, w} not found
	//	{
	//		if (_weights[idx] <= w)  // Weight if the current treasure does not exceed the weight capacity
	//		{
	//			// Check whether it is more optimal to include or exclude the current treasure
	//			unsigned exclude_last = optimal_value(n - 1, w);
	//			unsigned include_last = _values[idx] + optimal_value(n - 1, w - _weights[idx]);
	//			result = std::max(exclude_last, include_last);

	//			//// Add the included treasure index to the solution
	//			//if (include_last > exclude_last && !indexInSolution(idx))
	//			//	_solution.push_back(idx);
	//		}
	//		else
	//		{
	//			result = optimal_value(n - 1, w);  // Solve excluding the last treasure
	//		}

	//		memory.insert({ {n, w}, result });  // Memoize the result
	//	}
	//	else
	//	{
	//		result = it->second;
	//	}

	//	return result;
	//}

	// DO NOT INCLUDE
	/*bool indexInSolution(unsigned idx)
	{
		bool result = false;

		for (auto& i : _solution)
			if (i == idx)
				result = true;

		return result;
	}*/

	/// <summary>
	/// Calculates the indices of treasures and the optimal value for the given parameters.
	/// Makes use of memoization by populating memory map.
	/// </summary>
	/// <param name="n">- number of treasures left</param>
	/// <param name="w">- weight capacity left</param>
	std::vector<unsigned> optimal_solution(unsigned n, unsigned w)
	{
		if (n == 0 || w == 0)
			return {};

		std::vector<unsigned> result;
		unsigned idx = n - 1;  // Index of the current treasure
		auto it = memory.find({ n, w });

		if (it == memory.end())  // Pair {n, w} not found
		{
			if (_weights[idx] <= w)  // Weight if the current treasure does not exceed the weight capacity
			{
				std::vector<unsigned> exclude_last = optimal_solution(n - 1, w);
				
				std::vector<unsigned> include_last = optimal_solution(n - 1, w - _weights[idx]);
				include_last.push_back(idx);

				unsigned excluded_value = 0;
				unsigned included_value = 0;

				// Calculate values from excluding and including the current treasure
				for (auto& i : exclude_last) excluded_value += _values[i];
				for (auto& i : include_last) included_value += _values[i];

				// Store the optimal value
				_value = std::max(excluded_value, included_value);

				// Check whether it is more optimal to include or exclude the current treasure
				if (included_value > excluded_value)
					result = include_last;
				else
					result = exclude_last;
			}
			else
			{
				result = optimal_solution(n - 1, w);
			}
		}
		else
		{
			result = it->second;
		}

		return result;
	}
};

int main5_3()
{
	unsigned weight_limit = 10;
	std::cout << "Weight limit = " << weight_limit << "\n\n";

	// First example --------------------------------------------------
	std::vector<unsigned> values = { 8, 5, 6, 9, 7 };
	std::vector<unsigned> weights = { 1, 4, 3, 2, 5 };

	std::cout << "Values  = " << values << "\n";
	std::cout << "Weights = " << weights << "\n";
	
	Treasury treasury(values, weights, weight_limit);
	std::cout << "Optimal treasures' indices = " << treasury.solution() << "\n";
	std::cout << "Optimal value = " << treasury.value() << "\n\n";

	// Second example -------------------------------------------------
	std::cout << "Swapping values and weights of treasures indexed as 1 and 4:\n";
	unsigned v = values[1];
	values[1] = values[4];
	values[4] = v;
	unsigned w = weights[1];
	weights[1] = weights[4];
	weights[4] = w;

	std::cout << "Values  = " << values << "\n";
	std::cout << "Weights = " << weights << "\n";

	treasury(values, weights, weight_limit);
	std::cout << "Optimal treasures' indices = " << treasury.solution() << "\n";
	std::cout << "Optimal value = " << treasury.value() << "\n\n";

	// Third example --------------------------------------------------
	std::cout << "Setting new values and weights:\n";
	values = { 2, 5, 9, 1, 3 };
	weights = { 4, 2, 7, 1, 5 };

	std::cout << "Values  = " << values << "\n";
	std::cout << "Weights = " << weights << "\n";

	std::cout << "Optimal treasures' indices = " << treasury(values, weights, weight_limit) << "\n";  // operator() returns the solution vector
	std::cout << "Optimal value = " << treasury.value() << "\n\n";  // Optimal value has to be accessed separately


	return 0;
}