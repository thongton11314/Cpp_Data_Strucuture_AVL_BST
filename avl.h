/*
* Project: OrderedMap Project
* File: avl.h
* Edited: by Thai Tran, Thong Ton
* Date: 6/13/2019
* Description: AVL tree class
* Reference: Professor Paul Bladek
*/
#ifndef AVL_H
#define AVL_H
#include "bst.h"

using namespace std;

namespace TT_AVL
{
	//--------------------------------------------------------------------
	// AVL Tree
	//   Class: avl<T>: public bst<T>
	//   REQUIRES Type T be able convert from int & have < & == defined
	//
	//   File: avl.h
	//   Title: avl template Class -- inherits from bst<T>
	//   Description: This file contains the class definition for avl
	//
	//   Programmer:  Paul Bladek
	// 
	//   Date:  May 2008
	//   Version: 1.0
	// 
	//   Environment: Hardware: IBM Pentium 
	//   Software: Windows XP or .Net Framework for execution;
	//
	//   Compiles under Microsoft Visual C++.Net 2017
	// 
	//   class avl<T>:
	//
	//     Methods:
	// 
	//       inline: 
	//			avl() -- default constructor
	//			avl(const avl<T>& t) -- copy constructor (deep copy)
	//			avl<T>& operator+=(const T d) -- adds an element to the tree
	//			avl<T> operator+(const T d) -- adds an element to the tree
	//			void insert(T d) -- adds an element to the tree
	//			~avl() -- destructor	
	//
	//       non-inline:
	//			avl<T>& operator=(const avl<T>& t) -- deep copy
	//			avl<T>& operator+=(const avl<T>& t) -- adds an element to the tree
	//			void insert(T d, node<T>* &cur) -- adds an element to the tree
	//			T popnode(node<T>* &cur) -- returns element in node deleted
	//			T poplow(node<T>* &cur) -- returns element in leftmost node (deleted)
	//			T popHigh(node<T>* &cur) -- returns element in rightmost node (deleted)
	//			T popfirst(const T& d, node<T>* np)
	//				-- returns element in first node matching d (deleted)
	//		protected:
	//			node<T>* rotateRight(node<T> *nodeN) -- balances tree	
	//			node<T>* rotateLeft(node<T> *nodeN) -- balances tree
	//			node<T>* rotateRightLeft(node<T> *nodeN) -- balances tree
	//			node<T>* rotateLeftRight(node<T> *nodeN) -- balances tree
	//			node<T>* rebalance(node<T> *&nodeN) -- balances tree
	//			int getHeightDifference(const node<T> *const nodeN)const
	//				-- returns the height difference between the left & right subtrees 
	//
	//
	//   History Log: 
	//     May 20, 2008, PB  completed version 1.0 
	//		06/04/2018,  TT	completed version 2.0
	//------------------------------------------------------------------- 
	template<class T>
	class avl : public bst<T>
	{
	public:
		using bst<T>::bst;
		avl() : bst<T>::bst() {}
		avl(const avl<T>& t) : bst<T>::bst(t) {}
		avl<T>& operator=(const avl<T>& t);
		avl<T>& operator+=(const avl<T>& t);
		avl<T>& operator+=(const T d) { insert(d); return *this; }
		avl<T> operator+(const T d)
		{
			avl<T> temp = *this; temp.insert(d); return temp;
		}
		bool insert(T d) { return insert(d, this->root); }
		bool insert(T d, node<T>* &cur);
		T popNode(node<T>* &cur);
		T popLow(node<T>* &cur);
		T popHigh(node<T>* &cur);
		T popFirstOf(const T& d, node<T>*& np);
		T popFirstOf(const T& d) { return this->popFirstOf(d, this->root); }
		~avl() { this->delTree(); }
	protected:
		node<T>* rotateRight(node<T> *nodeN);
		node<T>* rotateLeft(node<T> *nodeN);
		node<T>* rotateRightLeft(node<T> *nodeN);
		node<T>* rotateLeftRight(node<T> *nodeN);
		node<T>* rebalance(node<T> *&nodeN);
		int getHeightDifference(const node<T> *const nodeN)const;
	};

	//--------------------------------------------------------------------
	// Title: operator=
	// Description: deep copy
	// Called By: bstDriver
	// Calls: isempty, node constructor, delTree
	// Parameters: t-- avl<T> that we want to make the deep copy
	// Returns: *this
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	avl<T>& avl<T>::operator=(const avl<T>& t)
	{
		//static_cast<bst<T>> this = static_cast<bst<T>> t;
		if (this != &t)
		{
			if (!this->isempty())
				this->delTree(this->root);
			if (!t.isempty())
			{
				this->root = new node<T>(*(t.root));
			}
		}
		return *this;
	}

	//--------------------------------------------------------------------
	// Title: operator+=
	// Description: adds an element to the tree
	// Called By: bstDriver
	// Calls: addTree
	// Parameters: t-- avl<T> that we want to add
	// Returns: *this
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	avl<T>& avl<T>::operator+=(const avl<T>& t)
	{
		this->addTree(t.root);
		//rebalance(t.root);
		return *this;
	}

	//--------------------------------------------------------------------
	// Title: insert
	// Description: insert an element to the node cur
	// Called By: insert, operator+=
	// Calls: rebalance, insert, setHeight, isempty
	// Parameters: t-- avl<T> that we want to add
	// Returns: true if we can insert it
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	bool avl<T>::insert(T d, node<T>* &cur)
	{
		if (cur == nullptr)
		{
			cur = new node<T>(d);
			if (this->isempty())
				this->root = cur;
			return true;
		}
		if (d < cur->value())
			insert(d, cur->left);
		else if (d > cur->value())
			insert(d, cur->right);
		else
			return false;
		if (this->root != nullptr)
			this->root->setHeight();
		rebalance(cur);
		return true;
	}

	//--------------------------------------------------------------------
	// Title: popnode
	// Description: delete a node
	// Called By: poplow, popfirst
	// Calls: setdata, popHigh, rebalance
	// Parameters: node<T>* &cur -- node that will be deleted
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::popNode(node<T>* &cur)
	{
		if (cur == nullptr)
			throw (invalid_argument("Pointer does not point to a node"));
		T contents = cur->value();
		if (cur->left == nullptr && cur->right == nullptr)
		{ // no children
			delete cur;
			cur = nullptr;
		}
		else if (cur->left == nullptr)
		{ // only right child
			node<T>* temp = cur->right;
			delete cur;
			cur = temp;
		}
		else if (cur->right == nullptr)
		{ // only left child
			node<T>* temp = cur->left;
			delete cur;
			cur = temp;
		}
		else
		{ // two children
			cur->setdata(this->popHigh(cur->left));
			// pops leftmost node of right child and
			// places that value into the current node
		}
		if (this->root != nullptr)
		{
			this->root->setHeight();
			rebalance(this->root); // new
		}

		return contents;
	}

	//--------------------------------------------------------------------
	// Title: poplow
	// Description: delete the lowest element
	// Called By: driver
	// Calls: setdata, poplow, rebalance
	// Parameters: node<T>* &cur -- node that will be deleted
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::popLow(node<T>* &cur)
	{
		T result;
		if (cur == nullptr)
		{
			throw (invalid_argument("Invalid pointer"));
		}

		if (cur->left == nullptr)
		{
			result = cur->value();
			node<T>* temp = cur->right;
			delete cur;

			cur = temp;
			if (this->root != nullptr)
			{
				this->root->setHeight();
			}
			rebalance(cur);
			return result;
		}

		result = this->popLow(cur->left);

		rebalance(cur);
		return result;
	}

	//--------------------------------------------------------------------
	// Title: popHigh
	// Description: delete the highest element
	// Called By: driver
	// Calls: setdata, poplow, rebalance
	// Parameters: node<T>* &cur -- node that will be deleted
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::popHigh(node<T>* &cur)
	{
		if (cur == nullptr)
			throw(invalid_argument("Pointer does not point to a node"));
		if (cur->right == nullptr)
		{
			T temp = cur->value();
			node<T>* temptr = cur->left;
			delete cur;
			cur = temptr;
			if (this->root != nullptr)
				this->root->setHeight();
			rebalance(cur);
			return temp;
		}
		T temp1 = this->popHigh(cur->right);
		rebalance(cur);
		return temp1;
	}

	//--------------------------------------------------------------------
	// Title: popFirstOf
	// Description: delete the element has data d
	// Called By: driver
	// Calls: popNode, popFirstOf, rebalance
	// Parameters: node<T>* &cur -- node that will be deleted
	//			const T& d -- data
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	T avl<T>::popFirstOf(const T& d, node<T>*& np)
	{
		T result = T(0);

		if (np != nullptr) {
			if (np->value() == d)
			{
				result = this->popNode(np);
			}

			else if (d < np->value())
			{
				result = this->popFirstOf(d, np->left);
			}

			else
			{
				result = this->popFirstOf(d, np->right);
			}

			this->rebalance(np);
		}
		//else
		//{
		//	throw invalid_argument("Item not found");
		//}
		return result;
	}

	//--------------------------------------------------------------------
	// Title: rotateRight
	// Description: balancing
	// Called By: rebalance
	// Calls: 
	// Parameters: node<T> *nodeN -- node that will be rotated
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	//right-right case
	template <class T>
	node<T>* avl<T>::rotateRight(node<T> *nodeN)
	{
		//right-right case
		node<T> *temp = nodeN->right;
		nodeN->right = temp->left;
		temp->left = nodeN;
		return temp;
	}

	//--------------------------------------------------------------------
	// Title: rotateLeft
	// Description: balancing
	// Called By: rebalance
	// Calls: 
	// Parameters: node<T> *nodeN -- node that will be rotated
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateLeft(node<T> *nodeN)
	{
		//left-left case
		node<T> *temp = nodeN->left;
		nodeN->left = temp->right;
		temp->right = nodeN;
		return temp;
	}

	//--------------------------------------------------------------------
	// Title: rotateRight
	// Description: balancing
	// Called By: rebalance
	// Calls: rotateLeft, rotateRight
	// Parameters: node<T> *nodeN -- node that will be rotated
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateRightLeft(node<T> *nodeN)
	{	//right-left case
		node<T> *temp = nodeN->right;
		nodeN->right = rotateLeft(temp);
		return rotateRight(nodeN);
	}

	//--------------------------------------------------------------------
	// Title: rotateRight
	// Description: balancing
	// Called By: rebalance
	// Calls: rotateLeft, rotateRight
	// Parameters: node<T> *nodeN -- node that will be rotated
	// Returns: T -- value of deleted element
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rotateLeftRight(node<T> *nodeN)
	{	//left-right case
		node<T> * temp = nodeN->left;
		nodeN->left = rotateRight(temp);
		return rotateLeft(nodeN);
	}

	//--------------------------------------------------------------------
	// Title: rebalance
	// Description: balancing
	// Called By: rebalance
	// Calls: rotateLeft, rotateRight, getHeightDifference,setHeight
	// Parameters: node<T> *nodeN -- node that will be rotated
	// Returns: nodeN
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	node<T>* avl<T>::rebalance(node<T> *&nodeN)
	{
		int heightDiff = getHeightDifference(nodeN);

		//left case
		if (heightDiff > 1)
		{
			//left right case
			if (getHeightDifference(nodeN->left) < 0)
				nodeN = rotateLeftRight(nodeN);
			else
				//left left case
				nodeN = rotateLeft(nodeN);
		}
		// right case
		else if (heightDiff < -1)
		{
			//right left case
			if (getHeightDifference(nodeN->right) > 0)
				nodeN = rotateRightLeft(nodeN);
			else
				//right right case
				nodeN = rotateRight(nodeN);
		}

		this->setHeight();
		return nodeN;
	}

	//--------------------------------------------------------------------
	// Title: getHeightDifference
	// Description: get height difference
	// Called By: rebalance
	// Calls: ,getHeight
	// Parameters: const node<T> *const nodeN -- node to get height
	// Returns: int leftHeight - rightHeight;
	// Exception: 
	// History:  06/4/2018: TT Compelted version 1.0
	//--------------------------------------------------------------------
	template <class T>
	int avl<T>::getHeightDifference(const node<T> *const nodeN)const
	{
		int leftHeight = 0;
		int rightHeight = 0;
		if (nodeN != nullptr) {
			if (nodeN->right != nullptr)
			{
				rightHeight = nodeN->right->getHeight();
			}

			if (nodeN->left != nullptr)
			{
				leftHeight = nodeN->left->getHeight();
			}
		}
		return leftHeight - rightHeight;
	}
}
#endif