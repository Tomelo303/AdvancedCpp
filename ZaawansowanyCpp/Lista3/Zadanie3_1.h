#pragma once

#include <iostream>
#include <vector>
#include <type_traits>


template <typename T>
concept HasSubscriptOperator = requires (T a)
{
	a[0];
};

template <typename T>
concept HasSizeMethod = requires (T a)
{
	a.size();
};

template <typename T>
concept IsValueTypeNumeric = std::is_arithmetic<typename T::value_type>::value;

template <typename T>
concept IsVector = HasSubscriptOperator<T> && HasSizeMethod<T> && IsValueTypeNumeric<T>;

// Wektor z listy 1
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
	template <IsVector V, IsVector U>
	auto operator*(const V& v, const U& u)
	{
		if (v.size() != u.size()) throw std::runtime_error("Vectors must be the same size");

		using ResultType = decltype(std::declval<typename V::value_type>() * std::declval<typename U::value_type>());  // Apparently writing result = v[0] * u[0] is enough

		ResultType result = ResultType();
		for (size_t i = 0; i < v.size(); i++)
		{
			result += v[i] * u[i];
		}

		return result;
	}
}


int main3_1()
{
	using cpplab::operator*;

	std::vector<double> dblVec = { 1.2, 3.4, 5.6, 7.8 };
	std::vector<std::string> strVec = { "a", "b", "c", "d" };
	cpplab::vector<int> intVec = { 1, 2, 3, 4 };
	cpplab::vector<bool> boolVec = { false, true, false, true };

	std::cout << "Dot product of dblVec and intVec = " << dblVec * intVec << "\n";
	std::cout << "Dot product of boolVec and intVec = " << boolVec * intVec << "\n";
	//std::cout << "Dot product of strVec and strVec = " << strVec * strVec << "\n";  // no operator "*" matches these operands
	//std::cout << "Dot product of strVec and intVec = " << strVec * intVec << "\n";  // no operator "*" matches these operands

	/*std::cout << std::is_arithmetic<decltype(dblVec)::value_type>::value << "\n";
	std::cout << std::is_arithmetic<decltype(strVec)::value_type>::value << "\n";
	std::cout << std::is_arithmetic<decltype(intVec)::value_type>::value << "\n";
	std::cout << std::is_arithmetic<decltype(boolVec)::value_type>::value << "\n";*/

	return 0;
}