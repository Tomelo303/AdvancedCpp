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
		vector()
		{
			std::cout << "Used my default constructor\n";
		}

		// Initializer list constructor
		vector(std::initializer_list<T> list)
			: _size(list.size()), _capacity(list.size())
		{
			std::cout << "Used my initializer list constructor\n";

			_data = new T[_capacity];

			size_t idx = 0;
			for (const T& elem : list)
			{
				_data[idx++] = elem;
			}
		}

		vector(size_t size, T default_value)
			: _size(size), _capacity(size)
		{
			_data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				_data[i] = default_value;
			}
		}

		// Copy constructor
		vector(const vector<T>& vec)
			: _size(vec._size), _capacity(vec._capacity)
		{
			std::cout << "Used my copy constructor\n";

			_data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				_data[i] = vec[i];
			}
		}

		// Move constructor
		vector(vector<T>&& vec) noexcept
		{
			std::cout << "Used my move constructor\n";

			_size = vec._size;
			_capacity = vec._capacity;
			_data = vec._data;

			vec._size = 0;
			vec._capacity = 0;
			vec._data = nullptr;
		}

		~vector()
		{
			std::cout << "Used my destructor\n";
			delete[] _data;
		}


		using value_type = T;

		size_t size() const { return _size; }
		size_t capacity() const { return _capacity; }
		bool empty() const { return _size == 0; }

		T at(size_t idx) const
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of bounds");
			return _data[idx];
		}

		T& at(size_t idx)
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of bounds");
			return _data[idx];
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
					_data[i] = 0;
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
					tmp[i] = _data[i];
				else
					tmp[i] = 0;
			}

			_capacity = new_capacity;

			delete[] _data;
			_data = tmp;
		}

		void pop_back()
		{
			resize(_size - 1);
		}

		void push_back(T value)
		{
			if (_size == _capacity)
			{
				if (empty())
				{
					resize(1);
					_data[0] = value;
				}
				else
				{
					reserve(2 * _size);
					_data[_size] = value;
					_size++;
				}
			}
			else
			{
				_data[_size] = value;
				_size++;
			}
		}

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

			delete[] _data;
			_data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				_data[i] = vec[i];
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

			delete[] _data;
			_data = vec._data;

			vec._size = 0;
			vec._capacity = 0;
			vec._data = nullptr;

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
		T* _data = nullptr;
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


int main4_1()
{
	namespace cpp = cpplab;  // Declaring an alias for convenience

	cpp::vector<int> vec0 = { 1, 2, 3, 4 };
	std::cout << "vec0 = " << vec0 << "\n\n";

	// Using copy constructor
	auto vec1 = vec0;
	std::cout << "vec1 = vec0:\nvec1 = " << vec1 << "\nvec0 = " << vec0 << "\n\n";

	// Using move constructor
	auto vec2 = std::move(vec1);
	std::cout << "vec2 = std::move(vec1):\nvec2 = " << vec2 << "\nvec1 = " << vec1 << "\n\n";

	// Using copy assignment operator
	vec1 = vec0;
	std::cout << "vec1 = vec0:\nvec1 = " << vec1 << "\nvec0 = " << vec0 << "\n\n";

	cpp::vector<double> vec3 = { 1.2, 3.4, 5.6, 7.8 };
	std::cout << "vec3 = " << vec3 << "\n";
	cpp::vector<double> vec4;
	std::cout << "vec4 = " << vec4 << "\n\n";

	// Using move assignment operator
	vec4 = std::move(vec3);
	std::cout << "vec4 = std::move(vec3):\nvec4 = " << vec4 << "\nvec3 = " << vec3 << "\n\n";

	
	return 0;
}
