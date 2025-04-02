#pragma once

#include <iostream>
#include <memory>


namespace cpplab
{
	template <typename T>
	class forward_list
	{
	  private:
		// A building block of the forward list, stores data and a pointer to the next node
		// (if a node is at the end of the forward list, the pointer is set to nullptr).
		struct Node
		{
			Node(T value, std::unique_ptr<Node> next) : data(value), next(std::move(next)) {}

			T data;  // Data stored in the node
			std::unique_ptr<Node> next;  // Pointer to the next node in the forward list
		};
		
		std::unique_ptr<Node> head = nullptr;  // First node of the forward list

	  public:
		/* Default constructor */
		forward_list() {}

		/* Initializer list constructor */
		forward_list(std::initializer_list<T> init_list)
		{
			auto it = init_list.end();

			while (it != init_list.begin())
			{
				--it;
				push_front(*it);
			}
		}

		/* Destructor */
		~forward_list() = default;  // std::unique_ptr handles deleting nodes recursively

		using value_type = T;

		/* True if forward list is empty. */
		bool empty() const { return head == nullptr; }

		/* Returns the first element. */
		T front() const { return head->data; }

		/* Add a value to the beginning. */
		void push_front(T value)
		{
			std::unique_ptr<Node> new_head( new Node(value, std::move(head)) );
			head = std::move(new_head);
		}

		/* Remove the value at the beginning. */
		void pop_front()
		{
			if (head)
				head = std::move(head->next);  // Moves the head one step forward, std::unique_ptr deletes the data left behind
		}

		/* Reverses the order of the forward list. */
		void reverse()
		{
			std::unique_ptr<Node> prev = nullptr;
			std::unique_ptr<Node> curr = std::move(head);
			std::unique_ptr<Node> upco = nullptr;

			while (curr)
			{
				upco = std::move(curr->next);
				curr->next = std::move(prev);
				prev = std::move(curr);
				curr = std::move(upco);
			}

			head = std::move(prev);
		}

		/* Erases all elements. */
		void clear()
		{
			// Moves the head forward until head == nullptr, std::unique_ptr deletes the data left behind
			while (head)
				head = std::move(head->next);
		}

		forward_list& operator=(std::initializer_list<T> init_list)
		{
			clear();

			auto it = init_list.end();

			while (it != init_list.begin())
			{
				--it;
				push_front(*it);
			}

			return *this;
		}

		// Not a member of the traditional forward list, but it is helpful for debugging
		friend std::ostream& operator<<(std::ostream& out, const forward_list<T>& flist)
		{
			if (!flist.empty())
			{
				Node* tmp = flist.head->next.get();

				out << "[" << flist.head->data;
				while (tmp)
				{
					out << ", " << tmp->data;
					tmp = tmp->next.get();
				}
				out << "]";
			}
			else
			{
				out << "[]";
			}

			return out;
		}
	};
}

int main3_3()
{
	cpplab::forward_list<int> flist = { 1, 2, 3, 4 };
	std::cout << flist << "\n";
	std::cout << "Empty = " << flist.empty() << "\n";
	std::cout << "Front = " << flist.front() << "\n";

	std::cout << "\nAdding a value to the front:\n";
	flist.push_front(0);
	std::cout << flist << "\n";

	std::cout << "\nDeleting the value at the front:\n";
	flist.pop_front();
	std::cout << flist << "\n";

	std::cout << "\nReversing the forward list:\n";
	flist.reverse();
	std::cout << flist << "\n";
	std::cout << "Front = " << flist.front() << "\n";

	std::cout << "\nClearing the forward list:\n";
	flist.clear();
	std::cout << flist << "\n";
	std::cout << "Empty = " << flist.empty() << "\n";

	std::cout << "\nAssigning new data to the forward list:\n";
	flist = { 10, 11, 12 };
	std::cout << flist << "\n";

	std::cout << "\nOverwriting exisitng data in the forward list:\n";
	flist = { 5, 10, 15 };
	std::cout << flist << "\n";


	return 0;
}