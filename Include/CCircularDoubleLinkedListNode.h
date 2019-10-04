#pragma once

#ifndef CCIRCULARDOUBLELINKEDLISTNODE_H
#define CCIRCULARDOUBLELINKEDLISTNODE_H

#include <iostream>
using namespace std;

// Class that represents a node in a Double Linked list
// This class is a template
template <class T>
class CCircularDoubleLinkedListNode
{
private:
	T *m_Data;
	CCircularDoubleLinkedListNode *m_Next;
	CCircularDoubleLinkedListNode *m_Prev;
	CCircularDoubleLinkedListNode *m_Parent; // UNUSED (for now)
	CCircularDoubleLinkedListNode *m_Child;  // UNUSED (for now)

public:
	CCircularDoubleLinkedListNode();
	CCircularDoubleLinkedListNode(T *data);
	~CCircularDoubleLinkedListNode();

	T *getData() { return m_Data; }
	void setData(T *data) { m_Data = data; }

	CCircularDoubleLinkedListNode *getNext() { return m_Next; }
	void setNext(CCircularDoubleLinkedListNode *next) { m_Next = next; }
	CCircularDoubleLinkedListNode *getPrev() { return m_Prev; }
	void setPrev(CCircularDoubleLinkedListNode *prev) { m_Prev = prev; }
	CCircularDoubleLinkedListNode *  getParent() { return m_Parent; }
	void setParent(CCircularDoubleLinkedListNode *parent) { m_Parent = parent; }
	CCircularDoubleLinkedListNode *  getChild() { return m_Child; }
	void setChild(CCircularDoubleLinkedListNode *child) { m_Child = child; }
};

/* 
 * -----------------------
 * TEMPLATE IMPLEMENTATION
 * -----------------------
 */

/*
*/
template <class T>
CCircularDoubleLinkedListNode<T>::CCircularDoubleLinkedListNode() :
	m_Data{ NULL },
	m_Next{ NULL },
	m_Prev{ NULL },
	m_Parent{ NULL },
	m_Child{ NULL }
{
	cout << "Constructor: CCircularDoubleLinkedListNode()" << endl;
}

/*
*/
template <class T>
CCircularDoubleLinkedListNode<T>::CCircularDoubleLinkedListNode(T *data) :
	m_Data{ data },
	m_Next{ NULL },
	m_Prev{ NULL },
	m_Parent{ NULL },
	m_Child{ NULL }
{
	cout << "Constructor: CCircularDoubleLinkedListNode()" << endl;
}

/*
*/
template <class T>
CCircularDoubleLinkedListNode<T>::~CCircularDoubleLinkedListNode()
{
	cout << "Destructor: CCircularDoubleLinkedListNode()" << endl;

	if (m_Data != NULL)
	{
		delete m_Data;
		m_Data = NULL;
	}

	// Do NOT free up resources of CCircularDoubleLinkedListNode objects
	// The CCircularDoubleLinkedList object is responsible for that
	// Only set the pointers to NULL
	m_Next = NULL;
	m_Prev = NULL;
	m_Parent = NULL;
	m_Child = NULL;
}

#endif // !CCIRCULARDOUBLELINKEDLISTNODE_H
