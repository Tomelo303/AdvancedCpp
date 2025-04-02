// Forward list as it was implemented in the second semester, but a bit upgraded :)

#pragma once

#include <iostream>


// A building block of the forward list, stores data and a pointer to the next node
// (if a node is at the end of the forward list, the pointer is set to nullptr)
template <typename T>
struct Node
{
	T value;			   // Data stored in the node
	Node* next = nullptr;  // Pointer to the next node in the forward list 
};

// Releases the memory taken by a forward list
template <typename T>
void release(Node<T>** phead)
{
	Node<T>* head = *phead;

	while (head != nullptr)
	{
		Node<T>* tmp = head;
		head = head->next;
		delete tmp;
	}

	*phead = nullptr;
}

template <typename T>
void reverse(Node<T>** phead)
{
	Node<T>* prev = nullptr;  // Previous node
	Node<T>* curr = *phead;   // Current node
	Node<T>* upco = nullptr;  // Upcoming node

	while (curr != nullptr)
	{
		upco = curr->next;  // Store the next node
		curr->next = prev;  // Make current node point to the previous one
		prev = curr;		// Set previous to the current one
		curr = upco;		// Set current to the next one
	}
	*phead = prev;  // Set the head to the previous node (because current node points to nullptr)
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Node<T>* head)
{
	out << "[" << head->value;
	head = head->next;
	while (head != nullptr)
	{
		out << ", " << head->value;  // print the value at current head position
		head = head->next;			 // move head to the next node
	}
	out << "]";

	return out;
}

int exec()
{
	Node<int>* n1 = new Node<int>{ 1 };
	Node<int>* n2 = new Node<int>{ 2, n1 };
	Node<int>* n3 = new Node<int>{ 3, n2 };
	Node<int>* head = new Node<int>{ 4, n3 };

	/*
	
	 0.1. newNode = new Node(1)
	 0.2. head = newNode
	 0.3. curr = head

	 1.1. newNode = new Node(2)
	 1.2. curr->next = newNode
	 1.3. curr = newNode

	 2.1. newNode = new Node(3)
	 2.2. curr->next = newNode
	 2.3. curr = newNode

	*/

	std::cout << head->value << "\n";
	std::cout << head << "\n";
	std::cout << head->value << "\n";
	reverse(&head);
	std::cout << head << "\n";

	release(&head);

	return 0;
}