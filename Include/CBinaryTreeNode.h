#pragma once

#include <iostream>
#include <vector>
using namespace std;

/* Binary Tree Node Template Class */
template <class T> 
class CBinaryTreeNode
{
private:
	// Pointers to Right and Left nodes
	CBinaryTreeNode<T> *m_leftNode;
	CBinaryTreeNode<T> *m_rightNode;

	// Pointer to the data stored in this node
	T * m_pData;

public:
	// Constructor and Destructor
	CBinaryTreeNode(T * val);
	~CBinaryTreeNode();

	// Tree-traversal methods receive an optional pointer to a std::vector or pointers to T
	// During tree-traversal, the data is copied to this vector
	//
	// Note: Ideally, these vectors should be of <T * const>, but C++ standard forbids containers of const elements
	//
	void inOrder(std::vector<T *>*pTreeDataPtrs = NULL);
	void preOrder(std::vector<T *>*pTreeDataPtrs = NULL);
	void postOrder(std::vector<T *>*pTreeDataPtrs = NULL);

	// Insert a new value in the tree (Recursive method)
	bool insert(T * val);

	// Recursively destroy the children nodes from this node
	void destroyTree();	
};

/* */
template <class T> 
CBinaryTreeNode<T>::CBinaryTreeNode(T * val) :
	m_pData{ val },
	m_leftNode{ NULL },
	m_rightNode{ NULL }
{
}

/* */
template <class T> 
CBinaryTreeNode<T>::~CBinaryTreeNode()
{
	destroyTree();
}

/* PostOrder recursive method to destroy the children nodes from this node */
template <class T> 
void CBinaryTreeNode<T>::destroyTree()
{
	if (m_leftNode != NULL)
	{
		m_leftNode->destroyTree();
		delete m_leftNode;
		m_leftNode = NULL;
	}

	if (m_rightNode != NULL)
	{
		m_rightNode->destroyTree();
		delete m_rightNode;
		m_rightNode = NULL;
	}
	
	// Delete data in the node
	if (m_pData != NULL)
	{
		delete m_pData;
		m_pData = NULL;
	}
}

/* In-Order tree-traversal processes: LEFT CHILD, CURRENT NODE, RIGHT CHILD */
template <class T>  
void CBinaryTreeNode<T>::inOrder(std::vector<T *>*pTreeDataPtrs)
{
	if (m_leftNode != NULL) {
		m_leftNode->inOrder();
	}

	// Class 'T' needs to override the "<<" operator
	// cout << *m_pData << endl;
	// Commented-out: This is called every frame, instead, let the caller use pTreeDataPtrs to display the info if needed

	// Optionally, add the pointer to an output vector
	if (pTreeDataPtrs != NULL)
	{
		pTreeDataPtrs->push_back(m_pData);
	}

	if (m_rightNode != NULL) {
		m_rightNode->inOrder(pTreeDataPtrs);
	}
}

/* Pre-Order tree-traversal processes: CURRENT NODE, LEFT CHILD, RIGHT CHILD */
template <class T> 
void CBinaryTreeNode<T>::preOrder(std::vector<T *>*pTreeDataPtrs)
{
	// Class 'T' needs to override the "<<" operator
	// cout << *m_pData << endl;
	// Commented-out: This is called every frame, instead, let the caller use pTreeDataPtrs to display the info if needed

	// Optionally, add the pointer to an output vector
	if (pTreeDataPtrs != NULL)
	{
		pTreeDataPtrs->push_back(m_pData);
	}

	if (m_leftNode != NULL) {
		m_leftNode->preOrder(pTreeDataPtrs);
	}
	if (m_rightNode != NULL) {
		m_rightNode->preOrder(pTreeDataPtrs);
	}
}

/* Post-Order tree-traversal processes: LEFT CHILD, RIGHT CHILD, CURRENT NODE */
template <class T> 
void CBinaryTreeNode<T>::postOrder(std::vector<T *>*pTreeDataPtrs)
{
	if (m_leftNode != NULL) {
		m_leftNode->postOrder(pTreeDataPtrs);
	}
	if (m_rightNode != NULL) {
		m_rightNode->postOrder(pTreeDataPtrs);
	}

	// Class 'T' needs to override the "<<" operator
	// cout << *m_pData << endl;
	// Commented-out: This is called every frame, instead, let the caller use pTreeDataPtrs to display the info if needed

	// Optionally, add the pointer to an output vector
	if (pTreeDataPtrs != NULL)
	{
		pTreeDataPtrs->push_back(m_pData);
	}
}

/* */
template <class T> 
bool CBinaryTreeNode<T>::insert(T * pVal)
{
	bool inserted = false;

	// IMPORTANT:
	// Compare the pointed-to value, not the pointer itself, so the overloaded '<' operator is executed, otherwise 
	// the new value won't be inserted in the correct position.
	if (*pVal < *m_pData) {
		if (m_leftNode == NULL) {
			m_leftNode = new CBinaryTreeNode<T>(pVal);
			inserted = true;
		}
		else {
			inserted = m_leftNode->insert(pVal);
		}
	}
	else if (*pVal > *m_pData) {
		if (m_rightNode == NULL) {
			m_rightNode = new CBinaryTreeNode<T>(pVal);
			inserted = true;
		}
		else {
			inserted = m_rightNode->insert(pVal);
		}
	}
	else
	{
		// *pVal == *m_pData stored in this node
		// We're not keeping duplicates in this implementation, element is discarded (not added to the tree)
		cout << "CBinaryTreeNode<T>::insert() : We're not keeping duplicates in this implementation, element is discarded (not added to the tree)" << endl;
	}

	return inserted;
}
