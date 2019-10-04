#pragma once

#include "CBinaryTreeNode.h"
#include <iostream>
#include <vector>
using namespace std;

/* Binary Tree Template Class */
template <class T> 
class CBinaryTree
{
public:
	// Public enum that holds the tree traversal methods
	enum BINARY_TREE_TRAVERSAL_ORDER
	{
		IN_ORDER = 0,
		PRE_ORDER,
		POST_ORDER
	};

private:
	// Pointer to root node of the tree
	CBinaryTreeNode<T> *m_rootNode;

public:
	// Constructor and Destructor
	CBinaryTree();
	~CBinaryTree();

	// Insert a new value in the tree
	bool insert(T * val);

	// Remove all nodes from the tree
	void clearTree();

	// Tree-traversal methods receive an *optional* pointer to a std::vector of pointers to 'T'
	// During tree-traversal, the data is copied to this vector.
	//
	// This is done to avoid having to modify this class for specific purposes. For example, if the tree nodes
	// contain pointers to C3DModel objetcs, we'd need to modify the render method to perform a dynamic_cast
	// to convert m_pData to a C3DModel, and we'd also need to pass a pointer to the COpenGLRenderer instance to
	// this class so they could be rendered. INSTEAD, we return the elements in a vector, and the caller 
	// can do whatever it wants with them.
	//
	// Note: Ideally, these vectors should be of <T * const>, but C++ standard forbids containers of const elements, mostly because of copy constructors
	// "xmemory0(585): error C2338: The C++ Standard forbids containers of const elements because allocator<const T> is ill - formed."
	//
	void inOrder(std::vector<T *>*pTreeDataPtrs = NULL);
	void preOrder(std::vector<T *>*pTreeDataPtrs = NULL);
	void postOrder(std::vector<T *>*pTreeDataPtrs = NULL);
};

/* */
template <class T >
CBinaryTree<T>::CBinaryTree() :
	m_rootNode{NULL}
{
	cout << "CBinaryTree() Constructor" << endl;
}

/* */
template <class T> 
CBinaryTree<T>::~CBinaryTree()
{
	cout << "CBinaryTree() Destructor" << endl;
	clearTree();
}

/* Delete all nodes in the tree */
template <class T>
void CBinaryTree<T>::clearTree()
{
	if (m_rootNode != NULL)
	{
		m_rootNode->destroyTree();
		delete m_rootNode;
		m_rootNode = NULL;
	}
}

/* Standard InOrder Binary Tree Traversal */
template <class T>  
void CBinaryTree<T>::inOrder(std::vector<T *>*pTreeDataPtrs)
{
	if (m_rootNode != NULL) {
		m_rootNode->inOrder(pTreeDataPtrs);
	}
}

/* Standard PreOrder Binary Tree Traversal */
template <class T > 
void CBinaryTree<T>::preOrder(std::vector<T *>*pTreeDataPtrs)
{
	if (m_rootNode != NULL) {
		m_rootNode->preOrder(pTreeDataPtrs);
	}
}

/* Standard PostOrder Binary Tree Traversal */
template <class T>  
void CBinaryTree<T>::postOrder(std::vector<T *>*pTreeDataPtrs)
{
	if (m_rootNode != NULL) {
		m_rootNode->postOrder(pTreeDataPtrs);
	}
}

/* Insert a new value in the tree */
template <class T>  
bool CBinaryTree<T>::insert(T * val)
{
	bool inserted = false;

	if (m_rootNode == NULL) {
		CBinaryTreeNode<T> *newNode = new CBinaryTreeNode<T>(val);
		m_rootNode = newNode;
		inserted = true;
	}
	else {
		inserted = m_rootNode->insert(val);
	}

	return inserted;
}
