#pragma once

#include <iostream>


namespace cpplab
{
	template <typename T>
	class forward_list
	{
		// A building block of the forward list, stores data and a pointer to the next node
		// (if a node is at the end of the forward list, the pointer is set to nullptr).
		template <typename T>
		struct Node
		{
			Node(T data) : data(data), next(nullptr) {}
			Node(T data, Node<T>* next) : data(data), next(next) {}

			T data;			// Data stored in the node
			Node<T>* next;  // Pointer to the next node in the forward list
		};

	public:
		/* Default constructor */
		forward_list() {}

		/* Initializer list constructor */
		forward_list(std::initializer_list<T> init_list)
			: _size(init_list.size())
		{
			Node<T>* newNode = nullptr;
			Node<T>* curr = nullptr;

			for (const T& elem : init_list)
			{
				newNode = new Node<T>(elem);

				if (head == nullptr)
				{
					head = newNode;
					curr = head;
				}
				else
				{
					curr->next = newNode;
					curr = newNode;
				}
			}
		}

		/* Default value constructor */
		forward_list(size_t n, T default_value)
			: _size(n)
		{
			Node<T>* newNode = nullptr;
			Node<T>* curr = nullptr;

			for (size_t i = 0; i < n; i++)
			{
				newNode = new Node<T>(default_value);

				if (head == nullptr)
				{
					head = newNode;
					curr = head;
				}
				else
				{
					curr->next = newNode;
					curr = newNode;
				}
			}
		}

		/* Destructor */
		~forward_list()
		{
			while (head != nullptr)
			{
				Node<T>* tmp = head;
				//std::cout << "tmp = " << tmp->value << "\n";
				head_ahead();
				/*if (head == nullptr)
					std::cout << "head is nullified\n";
				else
					std::cout << "head = " << head->value << "\n";*/
				delete tmp;
			}
		}

		/* Get the size of the forward list. */
		size_t size() const { return _size; }

		/* True if forward list is empty. */
		bool empty() const { return head == nullptr; }

		/* Return the data stored at the given index. */
		T at(size_t idx)
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of range");

			Node<T>* tmp = head;	  // Copy the pointer to the first node
			move_forward(idx, &tmp);  // Move tmp to the desired node
			return tmp->data;		  // Return the value stored in the node that tmp points to
		}
		T operator[](size_t idx) { return this->at(idx); }

		/* Set the value of the data stored at the given index. */
		void set(size_t idx, T value)
		{
			if (idx < 0 || idx >= _size) throw std::range_error("Provided index is out of range.");

			Node<T>* tmp = head;	  // Copy the pointer to the first node
			move_forward(idx, &tmp);  // Move tmp to the desired node
			tmp->data = value;		  // Set the new value
		}

		/* Insert a value after the element with the given index. */
		void insert_after(size_t idx, T value)
		{
			Node<T>* tmp = head;  // Copy the pointer to the first node

			move_forward(idx, &tmp);  // Move tmp to the desired node
			Node<T>* prev = tmp;	  // Get hold of the node after which a new one will be placed

			move_forward(1, &tmp);						 // Move tmp one step ahead
			Node<T>* newNode = new Node<T>(value, tmp);  // Create a node that points to tmp (that is now one node further)
			prev->next = newNode;						 // Make the previous node point to the new one
			_size++;
		}

		/* Erase the value after the element with the given index. */
		void erase_after(size_t idx)
		{
			Node<T>* tmp = head;  // Copy the pointer to the first node

			move_forward(idx, &tmp);  // Move tmp to the desired node
			Node<T>* prev = tmp;	  // Get hold of the node after which a node will be deleted

			move_forward(1, &tmp);		  // Move tmp one step ahead
			Node<T>* nodeToDelete = tmp;  // Get hold of the node that will be deleted

			prev->next = nodeToDelete->next;  // Make the previous node point to the one after the node that will be deleted
			delete nodeToDelete;
			_size--;
		}

		/* Insert a value to the beginning. */
		void push_front(T value)
		{
			Node<T>* new_head = new Node<T>(value, head);
			head = new_head;
			_size++;
		}

		/* Remove the value at the beginning. */
		void pop_front()
		{
			Node<T>* front = head;
			head_ahead();
			delete front;
			_size--;
		}

		/* Insert a value to the end. */
		void push_back(T value) { insert_after(_size - 1, value); }

		/* Remove the value at the end. */
		void pop_back() { erase_after(_size - 2); }

		/* Reverses the order of data stored in the forward list. */
		void reverse()
		{
			Node<T>* prev = nullptr;
			Node<T>* curr = head;
			Node<T>* upco = nullptr;

			while (curr != nullptr)
			{
				upco = curr->next;
				curr->next = prev;
				prev = curr;
				curr = upco;
			}

			head = prev;
		}

		friend std::ostream& operator<<(std::ostream& out, forward_list<T>& flist)
		{
			if (!flist.empty())
			{
				out << "[" << flist[0];
				for (int i = 1; i < flist.size(); i++)
					out << ", " << flist[i];
				out << "]";
			}
			else
			{
				out << "[]";
			}

			return out;
		}

		using value_type = T;


	private:
		/* Set the head to point to the next node. It's only here because I find the name funny. */
		void head_ahead() { head = head->next; }

		/* Move the pointer through the forward list this many steps. */
		void move_forward(size_t steps, Node<T>** ptr)
		{
			for (size_t i = 0; i < steps; i++)
				*ptr = (*ptr)->next;
		}

		Node<T>* head = nullptr;  // First node of the forward list
		size_t _size = 0;
	};
}

int flist_new_idx()
{
	namespace cpp = cpplab;

	cpp::forward_list<int> flist = { 1, 2, 3, 4 };
	std::cout << flist << "\n";

	std::cout << "\nSetting and accessing values manually:\n";
	flist.set(0, 5);
	std::cout << flist[0] << "\n";
	flist.set(1, 6);
	std::cout << flist[1] << "\n";
	flist.set(2, 8);
	std::cout << flist[2] << "\n";
	flist.set(3, 9);
	std::cout << flist[3] << "\n";

	std::cout << "\nInserting a new value after the element with index 1:\n";
	flist.insert_after(1, 7);
	std::cout << flist << "\n";

	std::cout << "\nErasing the value after the element with index 1:\n";
	flist.erase_after(1);
	std::cout << flist << "\n";

	std::cout << "\nInserting a value to the front:\n";
	flist.push_front(4);
	std::cout << flist << "\n";

	std::cout << "\nDeleting the value at the front:\n";
	flist.pop_front();
	std::cout << flist << "\n";

	std::cout << "\nAdding a value to the end:\n";
	flist.push_back(10);
	std::cout << flist << "\n";

	std::cout << "\nDeleting the value at the end:\n";
	flist.pop_back();
	std::cout << flist << "\n";

	std::cout << "\nReversing the forward list:\n";
	flist.reverse();
	std::cout << flist << "\n";

	return 0;
}