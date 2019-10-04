#pragma once

#ifndef CCIRCULARDOUBLE_LL_H
#define CCIRCULARDOUBLE_LL_H

#include <iostream>
using namespace std;

#include "CCircularDoubleLinkedListNode.h"

template <class T>
class CCircularDoubleLinkedList
{
private:
	CCircularDoubleLinkedListNode<T> * m_FirstNode;
	bool addNode(CCircularDoubleLinkedListNode<T> *newNode);

public:
	CCircularDoubleLinkedList();
	~CCircularDoubleLinkedList();

	bool isEmpty() const { return m_FirstNode == NULL; }
	bool createNode(T *newDataItem);
	CCircularDoubleLinkedListNode<T>* getFirstNode() const { return m_FirstNode; };
	CCircularDoubleLinkedListNode<T>* getLastNode() const;
};

/*
* -----------------------
* TEMPLATE IMPLEMENTATION
* -----------------------
*/

/*
*/
template <class T>
CCircularDoubleLinkedList<T>::CCircularDoubleLinkedList() :
	m_FirstNode{ NULL }
{
	cout << "Constructor: CCircularDoubleLinkedList()" << endl;
}

/*
*/
template <class T>
CCircularDoubleLinkedList<T>::~CCircularDoubleLinkedList()
{
	cout << "Destructor: CCircularDoubleLinkedList()" << endl;

	if (m_FirstNode != NULL) 
	{
		// Free all CCircularDoubleLinkedListNode<T> nodes
		CCircularDoubleLinkedListNode<T> *aux = m_FirstNode->getNext();
		CCircularDoubleLinkedListNode<T> *aux2 = NULL;

		while (aux != m_FirstNode)
		{
			aux2 = aux;
			aux = aux->getNext();
			delete aux2;
		}

		delete m_FirstNode;
		m_FirstNode = NULL;
	}
}

/*
*/
template <class T>
bool CCircularDoubleLinkedList<T>::createNode(T *newDataItem)
{
	cout << "CCircularDoubleLinkedList<T>::createNode()" << endl;

	CCircularDoubleLinkedListNode<T> *newNode = new CCircularDoubleLinkedListNode<T>(newDataItem);
	return addNode(newNode);
}

/*
*/
template <class T>
bool CCircularDoubleLinkedList<T>::addNode(CCircularDoubleLinkedListNode<T> *newNode)
{
	cout << "CCircularDoubleLinkedList<T>::addNode()" << endl;

	if (m_FirstNode == NULL)
	{
		m_FirstNode = newNode;
		m_FirstNode->setNext(m_FirstNode);
		m_FirstNode->setPrev(m_FirstNode);
	}
	else
	{
		newNode->setPrev(m_FirstNode->getPrev());
		m_FirstNode->getPrev()->setNext(newNode);
		newNode->setNext(m_FirstNode);
		m_FirstNode->setPrev(newNode);
	}

	return true;
}

/*
*/
template <class T>
CCircularDoubleLinkedListNode<T>* CCircularDoubleLinkedList<T>::getLastNode() const
{
	if (m_FirstNode != NULL)
	{
		return m_FirstNode->getPrev();
	}

	return NULL;
}

#endif // !CCIRCULARDOUBLE_LL_H
