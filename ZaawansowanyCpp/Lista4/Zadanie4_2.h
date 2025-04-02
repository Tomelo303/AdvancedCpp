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
			_data = new T[_capacity];

			for (size_t i = 0; i < _size; i++)
			{
				_data[i] = vec[i];
			}
		}

		// Move constructor
		vector(vector<T>&& vec) noexcept
		{
			_size = vec._size;
			_capacity = vec._capacity;
			_data = vec._data;

			vec._size = 0;
			vec._capacity = 0;
			vec._data = nullptr;
		}

		~vector() 
		{
			// Manually call the destructor for each element, because delete[] won't destruct objects created via placement new operator
			for (size_t i = 0; i < _size; i++)
				_data[i].~T();

			// Delete the block of raw memory
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
			if (new_size < _size)  // Delete excess elements
			{
				for (size_t i = new_size; i < _size; i++)
					_data[i].~T();
			}
			else if (new_size <= _capacity)  // Construct new elements using default constructor
			{
				for (size_t i = _size; i < new_size; i++)
					new (&_data[i]) T();
			}
			else  // Reserve more memory and construct new elements using default constructor
			{
				reserve(new_size);
				for (size_t i = _size; i < new_size; i++)
					new (&_data[i]) T();
			}

			_size = new_size;
		}

		void reserve(size_t new_capacity)
		{
			if (new_capacity < _capacity) throw std::invalid_argument("Cannot reserve a smaller amount than already reserved");

			T* tmp = new T[new_capacity];

			// Move the data to the new memory using placement new and delete old memory
			for (size_t i = 0; i < _size; i++)
			{
				new (&tmp[i]) T(std::move(_data[i]));  // Moving data to the exact memory location
				_data[i].~T();
			}

			_capacity = new_capacity;

			delete[] _data;  //  Freeing the memory block allocated with new[]
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
					_data[_size++] = value;
				}
			}
			else
			{
				_data[_size++] = value;
			}
		}

		template <typename... Args>
		void emplace_back(Args&&... args)
		{
			// Allocate new memory if vector is out of space
			if (_capacity == _size)
				reserve(_capacity == 0 ? 1 : 2 * _size);

			// Construct new object in a specified location by forwarding arguments from parameter pack
			new (&_data[_size]) T(std::forward<Args>(args)...);

			_size++;
		}

		T operator[](size_t idx) const { return this->at(idx); }
		T& operator[](size_t idx) { return this->at(idx); }

		// Copy assignment operator
		vector& operator=(const vector<T>& vec)
		{
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

class Pixel
{
public:
	Pixel() 
	{ 
		//std::cout << "Default constructor called\n";
	}
	Pixel(int r, int g, int b) : r(r), g(g), b(b)
	{ 
		std::cout << "Constructor called: " << *this << "\n";
	}

	friend std::ostream& operator<<(std::ostream& out, const Pixel& pixel)
	{
		out << "Pixel(" << pixel.r << ", " << pixel.g << ", " << pixel.b << ")";
		return out;
	}

	int r = 0;
	int g = 0;
	int b = 0;
};


int main4_2()
{
	namespace cpp = cpplab;  // Declaring an alias for convenience

	std::cout << "--- Initiating vector with 2 Pixel objects\n";
	cpp::vector<Pixel> pixels = { Pixel(50, 50, 50), Pixel(100, 100, 100) };
	std::cout << "--- Pushing back a Pixel object (calling constructor outside the method's body)\n";
	pixels.push_back(Pixel(150, 150, 150));
	std::cout << "--- Emplacing back a Pixel object (calling constructor inside the method's body)\n";
	pixels.emplace_back(200, 200, 200);

	std::cout << "\n" << pixels << "\n";


	return 0;
}
