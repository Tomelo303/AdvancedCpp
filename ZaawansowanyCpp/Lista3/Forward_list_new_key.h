#pragma once

#include <iostream>


namespace cpplab
{
	template <typename T>
	class forward_list
	{
		// A building block of the forward list, stores data and a pointer to the next node
		// (if a node is at the end of the forward list, the pointer is set to nullptr).
		struct Node
		{
			Node(T data) : data(data), next(nullptr) {}
			Node(T data, Node* next) : data(data), next(next) {}

			T data;			// Data stored in the node
			Node* next;  // Pointer to the next node in the forward list
		};

	public:
		/* Default constructor */
		forward_list() {}

		/* Initializer list constructor */
		forward_list(std::initializer_list<T> init_list)
		{
			Node* newNode = nullptr;
			Node* curr = nullptr;

			for (const T& elem : init_list)
			{
				newNode = new Node(elem);

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
		{
			Node* newNode = nullptr;
			Node* curr = nullptr;

			for (size_t i = 0; i < n; i++)
			{
				newNode = new Node(default_value);

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
				Node* tmp = head;
				//std::cout << "tmp = " << tmp->value << "\n";
				head_ahead();
				/*if (head == nullptr)
					std::cout << "head is nullified\n";
				else
					std::cout << "head = " << head->value << "\n";*/
				delete tmp;
			}
		}

		using value_type = T;

		/* True if forward list is empty. */
		bool empty() const { return head == nullptr; }

		/* Returns the first element */
		T front() const { return head->data; }

		/* Insert a value after the element with the given key. */
		void insert_after(T key, T value)
		{
			Node* tmp = head;

			move_to_key(key, &tmp);
			Node* prev = tmp;

			tmp = tmp->next;
			Node* newNode = new Node(value, tmp);
			prev->next = newNode;
		}

		/* Erase the value after the element with the given index. */
		void erase_after(T key)
		{
			Node* tmp = head;  // Copy the pointer to the first node

			move_to_key(key, &tmp);   // Move tmp to the desired node
			Node* prev = tmp;	  // Get hold of the node after which a node will be deleted

			tmp = tmp->next;			  // Move tmp one step ahead
			Node* nodeToDelete = tmp;  // Get hold of the node that will be deleted

			prev->next = nodeToDelete->next;  // Make the previous node point to the one after the node that will be deleted
			delete nodeToDelete;
		}

		/* Insert a value to the beginning. */
		void push_front(T value)
		{
			Node* new_head = new Node(value, head);
			head = new_head;
		}

		/* Remove the value at the beginning. */
		void pop_front()
		{
			Node* front = head;
			head_ahead();
			delete front;
		}

		/* Reverses the order of data stored in the forward list. */
		void reverse()
		{
			Node* prev = nullptr;
			Node* curr = head;
			Node* upco = nullptr;

			while (curr != nullptr)
			{
				upco = curr->next;
				curr->next = prev;
				prev = curr;
				curr = upco;
			}

			head = prev;
		}

		Node* append(forward_list<T>& other)
		{
			Node** phead = &head;

			while (*phead)
				phead = &(*phead)->next;

			*phead = other.head;

			other.head = nullptr;

			return head;
		}

		// Not a member of the traditional forward list, but it is helpful for debugging
		friend std::ostream& operator<<(std::ostream& out, const forward_list<T>& flist)
		{
			if (!flist.empty())
			{
				Node* tmp = flist.head->next;

				out << "[" << flist.head->data;
				while (tmp != nullptr)
				{
					out << ", " << tmp->data;
					tmp = tmp->next;
				}
				out << "]";
			}
			else
			{
				out << "[]";
			}

			return out;
		}

	private:
		Node* head = nullptr;  // First node of the forward list

		/* Set the head to point to the next node. It's only here because I find the name funny. */
		void head_ahead() { head = head->next; }

		/// <summary>
		/// Move the pointer through the forward list until the key is reached.
		/// </summary>
		/// <param name="key">- value to be reached</param>
		/// <param name="ptr">- pointer to the head</param>
		void move_to_key(T key, Node** ptr)
		{
			while ((*ptr)->next != nullptr && (*ptr)->data != key)
				*ptr = (*ptr)->next;

			if ((*ptr)->next == nullptr && (*ptr)->data != key) throw std::invalid_argument("The forward list does not contain the given key.");
		}
	};
}

int flist_new_key()
{
	namespace cpp = cpplab;

	cpp::forward_list<int> flist = { 1, 2, 3, 4 };
	std::cout << flist << "\n";
	std::cout << "Empty = " << flist.empty() << "\n";
	std::cout << "Front = " << flist.front() << "\n";

	std::cout << "\nInserting 5 after the element with key = 4\n";
	flist.insert_after(4, 5);
	std::cout << flist << "\n";

	std::cout << "\nErasing the value after key = 3\n";
	flist.erase_after(3);
	std::cout << flist << "\n";

	std::cout << "\nInserting a value to the front:\n";
	flist.push_front(0);
	std::cout << flist << "\n";

	std::cout << "\nDeleting the value at the front:\n";
	flist.pop_front();
	std::cout << flist << "\n";

	std::cout << "\nReversing the forward list:\n";
	flist.reverse();
	std::cout << flist << "\n";

	cpp::forward_list<int> flist2 = { 10, 11, 12, 13 };
	std::cout << "\nAppending a second forward list:\n";
	flist.append(flist2);
	std::cout << flist << "\n" << flist2 << "\n";

	std::cout << "\nAppending an empty forward list:\n";
	flist.append(flist2);
	std::cout << flist << "\n" << flist2 << "\n";

	std::cout << "\nAppending to an empty forward list:\n";
	flist2.append(flist);
	std::cout << flist << "\n" << flist2 << "\n";


	return 0;
}