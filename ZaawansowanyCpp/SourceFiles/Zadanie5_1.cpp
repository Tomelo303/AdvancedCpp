#include <iostream>
#include <memory>


namespace cpplab
{
	template <typename T>
	concept Comparable = requires (T a, T b)
	{
		a < b;
		a > b;
		a == b;
	};

	template <Comparable T>
	class bst
	{
	private:
		struct Node
		{
			Node(T value, std::shared_ptr<Node> parent, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
				: value(value), parent(parent), left(left), right(right) {}

			T value;
			std::shared_ptr<Node> parent;
			std::shared_ptr<Node> left;
			std::shared_ptr<Node> right;
		};

		std::shared_ptr<Node> root;

	public:
		bst() {}
		bst(T value) : root(std::make_shared<Node>(value, nullptr, nullptr, nullptr)) {}
		bst(std::initializer_list<T> inilist)
		{
			for (const auto& elem : inilist)
				add_node(elem);
		}

		void add_node(const T& value)
		{
			if (root != nullptr)
			{
				std::shared_ptr<Node> node = search_node_for_adding(value, root);
				std::shared_ptr<Node> newNode = std::make_shared<Node>(value, node, nullptr, nullptr);

				if (value > node->value)
				{
					node->right = newNode;
				}
				else if (value < node->value)
				{
					node->left = newNode;
				}
				else
				{
					if (node->left != nullptr)
						node->right = newNode;
					else
						node->left = newNode;  // Left-biased
				}
			}
			else
			{
				root = std::make_shared<Node>(value, nullptr, nullptr, nullptr);
			}
		}

		void print_in_order()
		{
			std::cout << "[";
			print_in_order(root);
			std::cout << " ]";
		}

	private:
		/// <summary>Searches for a suitable node beneath which a new node with a given value can be added.</summary>
		/// <param name="value">- value to be added</param>
		/// <param name="start_node">- node at which the search is started</param>
		/// <returns>Pointer to the node beneath which a new node can be added</returns>
		std::shared_ptr<Node> search_node_for_adding(const T& value, std::shared_ptr<Node> start_node)
		{
			if (value > start_node->value)
			{
				if (start_node->right != nullptr)
					return search_node_for_adding(value, start_node->right);
				else
					return start_node;
			}
			else if (value < start_node->value)
			{
				if (start_node->left != nullptr)
					return search_node_for_adding(value, start_node->left);
				else
					return start_node;
			}
			else
			{
				if (start_node->left != nullptr && start_node->right != nullptr)
					return search_node_for_adding(value, start_node->left);  // Left-biased
				else
					return start_node;
			}
		}

		void print_in_order(std::shared_ptr<Node> node)
		{
			if (node == nullptr)
				return;

			print_in_order(node->left);
			std::cout << " " << node->value;
			print_in_order(node->right);
		}
	};
}


int main()
{
	namespace cpp = cpplab;

	cpp::bst bst = { 6, 8, 4, 7, 5, 6, 3, 4 };
	bst.print_in_order();

	return 0;
}