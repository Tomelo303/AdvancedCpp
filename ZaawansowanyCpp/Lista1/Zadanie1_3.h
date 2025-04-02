#pragma once

#include <iostream>
#include <vector>
#include <type_traits>


namespace cpplab {

	template <typename T>
	class vector
	{
	public:
		// Default constructor
		vector() {}

		// Initializer list constructor
		vector(std::initializer_list<T> list)
			: _size(list.size()), _capacity(list.size())
		{
			data = new T[_capacity];

			size_t idx = 0;
			for (const T& elem : list)
			{
				data[idx++] = elem;
			}
		}

		vector(size_t size, T default_value)
			: _size(size), _capacity(size)
		{
			data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				data[i] = default_value;
			}
		}

		// Copy constructor
		vector(const vector<T>& vec)
			: _size(vec._size), _capacity(vec._capacity)
		{
			std::cout << "Used my copy constructor\n";

			data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				data[i] = vec[i];
			}
		}

		// Move constructor
		vector(vector<T>&& vec) noexcept
		{
			std::cout << "Used my move constructor\n";

			_size = vec._size;
			_capacity = vec._capacity;
			data = vec.data;

			vec._size = 0;
			vec._capacity = 0;
			vec.data = nullptr;
		}

		~vector() { delete[] data; }


		using value_type = T;

		size_t size() const { return _size; }
		size_t capacity() const { return _capacity; }
		bool empty() const { return _size == 0; }

		T at(size_t idx) const
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of bounds");
			return data[idx];
		}

		T& at(size_t idx)
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of bounds");
			return data[idx];
		}

		void resize(size_t new_size)
		{
			if (new_size < _capacity)
			{
				if (new_size <= _size)
				{
					_size = new_size;
					return;
				}

				for (size_t i = _size; i < new_size; i++)
				{
					data[i] = 0;
				}
				_size = new_size;
				return;
			}
			else
			{
				reserve(new_size);
				_size = new_size;
			}
		}

		void reserve(size_t new_capacity)
		{
			if (new_capacity < _capacity) throw std::invalid_argument("Cannot reserve a smaller amount than already reserved");

			T* tmp = new T[new_capacity];

			for (size_t i = 0; i < new_capacity; i++)
			{
				if (i < _size)
					tmp[i] = data[i];
				else
					tmp[i] = 0;
			}

			_capacity = new_capacity;

			delete[] data;
			data = tmp;
		}

		void pop_back()
		{
			resize(_size - 1);
		}

		void pop() { pop_back(); }  // A pop_back alias, because I prefer the name pop :)

		void push_back(T value)
		{
			if (_size == _capacity)
			{
				if (empty())
				{
					resize(1);
					data[0] = value;
				}
				else
				{
					reserve(2 * _size);
					data[_size] = value;
					_size++;
				}
			}
			else
			{
				data[_size] = value;
				_size++;
			}
		}

		void append(T value) { push_back(value); }  // A push_back alias, because I prefer the name append :)

		T operator[](size_t idx) const { return this->at(idx); }
		T& operator[](size_t idx) { return this->at(idx); }

		// Copy assignment operator
		vector& operator=(const vector<T>& vec)
		{
			std::cout << "Used my copy assignment operator\n";

			if (this == &vec)
			{
				return *this;
			}

			_size = vec._size;
			_capacity = vec._capacity;

			delete[] data;
			data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				data[i] = vec[i];
			}

			return *this;
		}

		// Move assignment operator
		vector& operator=(vector<T>&& vec) noexcept
		{
			std::cout << "Used my move assignment operator\n";

			if (this == &vec)
			{
				return *this;
			}

			_size = vec._size;
			_capacity = vec._capacity;

			delete[] data;
			data = vec.data;

			vec._size = 0;
			vec._capacity = 0;
			vec.data = nullptr;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& out, const vector<T>& vec)
		{
			if (vec._size > 0)
			{
				out << "[" << vec[0];
				for (size_t i = 1; i < vec._size; i++)
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

		void print_info() const
		{
			std::cout << "size=" << _size << "; capacity=" << _capacity << "; data=" << *this;
		}

	private:
		size_t _size = 0;
		size_t _capacity = 0;
		T* data = nullptr;
	};

	// Scalar multiplacation operator
	template <typename V, typename U>
	auto operator*(const V& v, const U& u)
	{
		if (v.size() != u.size()) throw std::runtime_error("Vectors must be the same size");

		using ResultType = decltype(std::declval<typename V::value_type>()* std::declval<typename U::value_type>());

		ResultType result = ResultType();
		for (size_t i = 0; i < v.size(); i++)
		{
			result += v[i] * u[i];
		}

		return result;
	}
}


int main1_3()
{
	namespace cpp = cpplab;  // Declaring an alias for convenience
	using cpp::operator*;  // Making the scalar multiplication operator visible for std::vector

	auto vec0 = cpp::vector<double>(3, 2);

	decltype(vec0)::value_type x = 1.5;

	// Showing off all possible ways of overwriting existing values and printing them in the console
	vec0.at(0) = 9;
	std::cout << "vec0: " << vec0.at(0) << ", " << vec0.at(1) << ", " << vec0.at(2) << "\n";
	vec0[1] = 5.5;
	std::cout << "vec0: " << vec0[0] << ", " << vec0[1] << ", " << vec0[2] << "\n";
	vec0.operator[](2) = 37.9;
	std::cout << "vec0: " << vec0 << "\n\n";

	// Using copy constructor
	auto vec1 = vec0;
	std::cout << "Using copy constructor\nvec1 = vec0: " << vec1 << "\n\n";

	// Using move constructor
	auto vec2 = std::move(vec1);
	std::cout << "Using move constructor\nvec2 = vec1: " << vec2 << "\n";
	std::cout << "vec1: " << vec1 << "\n\n";

	// Using copy assignment operator
	vec1 = vec0;
	std::cout << "Using copy assignment operator\nvec1 = vec0: " << vec1 << "\n\n";

	// Using move assignment operator
	vec2[0] = vec2[1] = vec2[2] = 3.2;
	vec1 = std::move(vec2);
	std::cout << "Using move assignment operator\nvec1 = vec2: " << vec1 << "\n";
	std::cout << "vec2: " << vec2 << "\n\n";

	// Using initializer list constructor
	cpp::vector<int> vec3 = { 1, 2, 3, 4 };
	std::cout << "Using initializer list constructor\nvec3: " << vec3 << "\n\n";

	// Using default constructor
	cpp::vector<int> vec4;
	std::cout << "Using default constructor\nvec4: "; vec4.print_info(); std::cout << "\n\n";

	// Manipulating the size and capacity
	vec4.resize(5);
	std::cout << "Manipulating the size and capacity\nvec4: "; vec4.print_info(); std::cout << "\n";
	vec4[0] = 1;
	vec4[1] = 2;
	vec4[2] = 3;
	vec4[3] = 4;
	vec4[4] = 5;
	vec4.resize(7);
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n";
	vec4.resize(4);
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n";
	vec4.pop();
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n";
	vec4.resize(6);
	vec4[5] = 6;
	vec4.append(7);
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n";
	vec4.append(8);
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n";
	vec4.reserve(vec4.capacity() + 1);
	std::cout << "vec4: "; vec4.print_info(); std::cout << "\n\n";

	// Demonstrating the capacity's growth policy
	std::cout << "Demonstrating the capacity's growth policy\n";
	cpp::vector<int> vec5;
	std::cout << "vec5 initial values: "; vec5.print_info(); std::cout << "\n";
	for (int i = 0; i <= 32; i++)
	{
		size_t cap = vec5.capacity();
		vec5.append(i);
		if (vec5.capacity() != cap)
		{
			std::cout << "vec5: "; vec5.print_info(); std::cout << "\n";
		}
	}
	std::cout << "\n";

	// Scalar multiplication operator
	cpp::vector<double> vec6 = { 6.5, 7.1, 8.9, 9.2 };
	std::vector<double> vec = { 1.2, 3.4, 5.6, 7.8 };
	std::cout << "Dot product of vec3 (int) and vec6 (double): " << vec3 * vec6 << "\n";
	std::cout << "Dot product of vec3 (int) and vec (double; std): " << vec3 * vec << "\n";
	std::cout << "Dot product of vec (double; std) and vec3 (int): " << vec * vec3 << "\n";
	std::cout << "Dot product of vec (double; std) and vec (double; std): " << vec * vec << "\n";

	return 0;
}
