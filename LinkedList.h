#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include <stdexcept>

template <typename T>
struct Node
{
	T data;
	Node<T>* next;
};

template <typename T>
class LinkedList
{
private:
	Node<T>* head;
	int count;
public:
	LinkedList()
	{
		head = NULL;
		count = 0;
	}

	~LinkedList()
	{
		while (count > 0)
			removeAt(0);
	}

	void insertFront(const T data)
	{
		Node<T>* newNode = new Node<T>{ data, head };
		head = newNode;
		count++;
	}

	void insertAt(int index, const T data)
	{
		if (index < 0 || index > count)
			throw std::out_of_range("index out of range");
		if (index == 0)
		{
			insertFront(data);
			return;
		}
		Node<T>* temp = head;
		for (int i = 1; i < index; i++)
			temp = temp->next;
		Node<T>* newNode = new Node<T>{ data, temp->next };
		temp->next = newNode;
		count++;
	}

	T get(int index)
	{
		if (index < 0 || index >= count)
			throw std::out_of_range("index out of range");
		Node<T>* temp = head;
		for (int i = 1; i <= index; i++)
			temp = temp->next;
		return temp->data;
	}

	bool search(const T x)
	{
		Node<T>* current = head; // Initialize current  
		while (current != NULL)
		{
			if (current->data == x)
				return true;
			current = current->next;
		}
		return false;
	}

	int getIndex(const T data)
	{
		Node<T>* current = head; // Initialize current  
		int count = 0;
		while (current != NULL)
		{
			if (current->data == data)
				return count;
			current = current->next;
			count++;
		}
		throw std::out_of_range("Invalid data");
	}

	void removeAt(int index)
	{
		if (index < 0 || index >= count)
			throw std::out_of_range("index out of range");
		Node<T>* curr = head;
		if (index == 0)
		{
			head = curr->next;
			delete(curr);
		}
		else
		{
			for (int i = 1; i < index; i++)
				curr = curr->next;
			Node<T>* temp = curr->next->next;
			delete(curr->next);
			curr->next = temp;
		}
		count--;
	}

	void removeValue(const T data)
	{
		int count = 0;
		Node<T>* current = head; // Initialize current  
		while (current != NULL)
		{
			count++;
			if (current->data == data)
				removeAt(count);
			current = current->next;
		}
	}

	int getCount() const
	{
		return count;
	}
};

#endif