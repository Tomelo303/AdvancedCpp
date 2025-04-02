#pragma once

#include <iostream>
#include <type_traits>


namespace cpplab
{	
	// !!! It does not prevent setting the pointer to nullptr through a constructor, it only prevents setting std::nullptr_t as a template variable !!!
	/// <summary>Checks whether a type is same as nullptr after removing const, volatile and reference qualifiers.</summary>
	template <typename T>
	concept NonNullptr = !std::is_same_v<std::nullptr_t, std::remove_cvref_t<T>>;

	template <typename T>
	class unique_ptr
	{
	  private:
		T* pointer = nullptr;

	  public:
		unique_ptr() noexcept {}
		explicit unique_ptr(T* p) noexcept : pointer(p) {}
		unique_ptr(const unique_ptr& u) = delete;
		unique_ptr(unique_ptr&& u) noexcept
		{
			std::cout << "Move constructor called.\n";
			pointer = u.pointer;
			u.pointer = nullptr;
		}

		~unique_ptr() 
		{
			std::cout << "Destructor called.\n";
			delete pointer;
		}

		unique_ptr& operator=(T* p) = delete;
		unique_ptr& operator=(const unique_ptr& u) = delete;
		unique_ptr& operator=(unique_ptr&& u) noexcept 
		{
			std::cout << "Move assignment operator called.\n";

			if (this == &u)
				return *this;

			delete pointer;
			pointer = u.pointer;
			u.pointer = nullptr;
			return *this;
		}

		T operator*() const noexcept { return *pointer; }
		T* operator->() const noexcept { return pointer; }

		operator bool() const noexcept { return pointer != nullptr; }

		T* get() const noexcept { return pointer; }
	};

	// Alias for an unique_ptr with NonNullptr constraint
	/*template <NonNullptr T>
	using non0_ptr = unique_ptr<T>;*/

	/// <summary>Unique pointer that does not accept nullptr.</summary>
	template <NonNullptr T>
	class non0_ptr
	{
	  private:
		T* pointer = nullptr;

	  public:
		non0_ptr() = delete;
		explicit non0_ptr(T* p) noexcept : pointer(p) {}
		non0_ptr(std::nullptr_t p) = delete;
		non0_ptr(const non0_ptr& n) = delete;
		non0_ptr(non0_ptr&& n) noexcept
		{
			std::cout << "Move constructor called.\n";
			pointer = n.pointer;
			n.pointer = nullptr;  // I am thinking about calling a destructor for moved non0_ptr to prevent it from pointing to nullptr, but I do not know if it is the intended solution or even a safe thing to do
		}

		~non0_ptr()
		{
			std::cout << "Destructor called.\n";
			delete pointer;
		}

		non0_ptr& operator=(T* p) = delete;
		non0_ptr& operator=(const non0_ptr& n) = delete;
		non0_ptr& operator=(non0_ptr&& n) noexcept
		{
			std::cout << "Move assignment operator called.\n";

			// Prevents nullifying itself and setting nullptr from non0_ptr which points to nullptr (e.g. after it was moved)
			if (this == &n || !n)
				return *this;

			delete pointer;
			pointer = n.pointer;
			n.pointer = nullptr;
			return *this;
		}

		T operator*() const noexcept { return *pointer; }
		T* operator->() const noexcept { return pointer; }

		operator bool() const noexcept { return pointer != nullptr; }

		T* get() const noexcept { return pointer; }
	};
}

// Test object
struct obj
{
	int i;
	double d;

	friend std::ostream& operator<<(std::ostream& out, const obj& o) { return out << "obj(" << o.i << ", " << o.d << ")"; }
};


int main5_2()
{
	namespace cpp = cpplab;


	obj* o1 = new obj{ 1, 2.5 };
	obj* o2 = new obj{ 2, 3.5 };

	cpp::unique_ptr<obj> u1(o1);
	cpp::unique_ptr<obj> u2(o2);

	std::cout << "Values held by " << u1.get() << " that u1 points to: " << u1->i << ", " << u1->d << "\n";
	std::cout << "Values held by " << u2.get() << " that u2 points to: " << u2->i << ", " << u2->d << "\n";

	//u1 = u2;  // Trying to call copy assignment operator results in an error: It is a deleted function
	
	// Calling move assignment operator
	u1 = std::move(u2);

	u1 ? (std::cout << "u1 points to " << u1.get() << ": " << *u1 << "\n")
	   : (std::cout << "u1 is nullptr\n");

	u2 ? (std::cout << "u2 points to " << u2.get() << ": " << *u2 << "\n")
	   : (std::cout << "u2 is nullptr\n");

	std::cout << "\n";

	//cpp::unique_ptr<int> u3 = new int(37);  // Due to the use of explicit keyword an error occurs: no suitable constructor exists
	cpp::unique_ptr<int> u3(new int(37));

	u3 ? (std::cout << "u3 points to " << u3.get() << ": " << *u3 << "\n")
	   : (std::cout << "u3 is nullptr\n");

	//cpp::unique_ptr u4 = u3;  // Trying to call copy constructor results in an error: It is a deleted function

	// Calling move constructor
	cpp::unique_ptr u4 = std::move(u3);

	u4 ? (std::cout << "u4 points to " << u4.get() << ": " << *u4 << "\n")
	   : (std::cout << "u4 is nullptr\n");

	u3 ? (std::cout << "u3 points to " << u3.get() << ": " << *u3 << "\n")
	   : (std::cout << "u3 is nullptr\n");

	//u4 = new int(73);  // Trying to call assignment operator with another pointer results in an error: It is a deleted function

	std::cout << "\n";

	//cpp::non0_ptr<std::nullptr_t> n0(nullptr);  // Error: Template constraint not satisfied
	
	cpp::unique_ptr<std::nullptr_t> u5(nullptr);  // I just now realized that unique_ptr with template parameter std::nullptr_t is basically useless
	u5 ? (std::cout << "u5 points to " << u5.get() << ": " << *u5 << "\n")
	   : (std::cout << "u5 is nullptr\n");

	cpp::non0_ptr n1(new int(99));
	n1 ? (std::cout << "n1 points to " << n1.get() << ": " << *n1 << "\n")
	   : (std::cout << "n1 is nullptr\n");

	//cpp::non0_ptr<int> n2(nullptr);  // Trying to call a constructor with nullptr results in an error: It is a deleted function

	cpp::non0_ptr<int> n2(new int(1));

	n1 = std::move(n2);
	n1 ? (std::cout << "n1 points to " << n1.get() << ": " << *n1 << "\n")
	   : (std::cout << "n1 is nullptr\n");

	n1 = std::move(n2);  // n2 points to nullptr
	n1 ? (std::cout << "n1 points to " << n1.get() << ": " << *n1 << "\n")
	   : (std::cout << "n1 is nullptr\n");

	std::cout << "\n";


	return 0;
}