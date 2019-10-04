#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "../Include/CGameMenu.h"

/* */
CGameMenu::CGameMenu() :
	m_Active{false},
	m_Initialized{false},
	m_MenuItemWidth{DEFAULT_MENU_ITEM_WIDTH},
	m_MenuItemHeight{DEFAULT_MENU_ITEM_HEIGHT},
	m_shaderProgramId{ 0 },
	m_textureObjectId{ 0 }
{
	cout << "Constructor: CGameMenu()" << endl;

	m_MenuItemList = new CCircularDoubleLinkedList<CGameMenuItem>();
}

/* */
CGameMenu::~CGameMenu()
{
	cout << "Destructor: CGameMenu()" << endl;

	m_Active = false;

	// Free resources of m_MenuItemList (all nodes)
	if (m_MenuItemList != NULL)
	{
		delete m_MenuItemList;
		m_MenuItemList = NULL;
	}
}

/* */
void CGameMenu::render(COpenGLRenderer * const renderer)
{
	if (m_MenuItemList != NULL)
	{
		CCircularDoubleLinkedListNode<CGameMenuItem> *firstNode = m_MenuItemList->getFirstNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *lastNode = m_MenuItemList->getLastNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *currentNode = firstNode;
		CGameMenuItem *menuItem = NULL;
		float color[3] = { 1.0f, 1.0f, 1.0f };

		if (firstNode == NULL)
		{
			return;
		}

		menuItem = firstNode->getData();
		if (menuItem != NULL)
		{
			menuItem->getColor()->getColorfv(color);

			renderer->renderMenuItem(
				&m_shaderProgramId,
				&m_textureObjectId,
				menuItem->getArrayObjectID(),
				color
			);
		}

		// If Menu has more than one node
		if (firstNode != lastNode)
		{
			currentNode = currentNode->getNext();

			while (currentNode != firstNode)
			{
				menuItem = currentNode->getData();
				if (menuItem != NULL)
				{
					menuItem->getColor()->getColorfv(color);

					renderer->renderMenuItem(
						&m_shaderProgramId,
						&m_textureObjectId,
						menuItem->getArrayObjectID(),
						color
					);
				}

				currentNode = currentNode->getNext();
			}
		}
	}
}

/* */
void CGameMenu::cleanupGraphicsObjects(COpenGLRenderer * const renderer)
{
	if (m_MenuItemList != NULL)
	{
		cout << "CGameMenu::cleanupGraphicsObjects()" << endl;

		// Delete texture object
		renderer->deleteTexture(&m_textureObjectId);

		CCircularDoubleLinkedListNode<CGameMenuItem> *firstNode = m_MenuItemList->getFirstNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *lastNode = m_MenuItemList->getLastNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *currentNode = firstNode;
		CGameMenuItem *menuItem = NULL;

		if (firstNode != NULL)
		{
			menuItem = firstNode->getData();
			if (menuItem != NULL)
			{
				cout << "deleting menu item VAO: " << *(menuItem->getArrayObjectID()) << endl;
				renderer->freeGraphicsMemoryForObject(/*&m_shaderProgramId,*/ menuItem->getArrayObjectID());
			}
		}

		// If Menu has more than one node
		if (firstNode != lastNode)
		{
			currentNode = currentNode->getNext();

			while (currentNode != firstNode)
			{
				menuItem = currentNode->getData();
				if (menuItem != NULL)
				{
					cout << "deleting menu item VAO: " << *(menuItem->getArrayObjectID()) << endl;
					renderer->freeGraphicsMemoryForObject(/*&m_shaderProgramId,*/ menuItem->getArrayObjectID());
				}

				currentNode = currentNode->getNext();
			}
		}
	}
}

/* */ // TO-DO, remove x and y, not needed anymore
void CGameMenu::addMenuItem(const char * const menuLabel, float x, float y, unsigned int vaoId/*, CMenuItem *parent*/)
{
	cout << "CGameMenu::addMenuItem()" << endl;

	if (m_MenuItemList != NULL)
	{
		bool firstNode = m_MenuItemList->isEmpty();
		CGameMenuItem *newMenuItem = new CGameMenuItem(menuLabel, x, y, vaoId);
		
		if (firstNode)
		{
			newMenuItem->setSelected(true);
		}

		m_MenuItemList->createNode(newMenuItem);

		if (!m_Initialized)
		{
			m_Initialized = true;
		}
	}
}

/* */
CCircularDoubleLinkedListNode<CGameMenuItem> * CGameMenu::getSelectedMenuNode(int *selectedItemNumber) const
{
	int currentItem = 1;
	CCircularDoubleLinkedListNode<CGameMenuItem> *selectedNode = NULL;

	if (m_MenuItemList != NULL && selectedItemNumber != NULL)
	{
		*selectedItemNumber = -1;
		CCircularDoubleLinkedListNode<CGameMenuItem> *firstNode   = m_MenuItemList->getFirstNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *lastNode    = m_MenuItemList->getLastNode();
		CCircularDoubleLinkedListNode<CGameMenuItem> *currentNode = firstNode;
		CGameMenuItem *menuItem = NULL;

		// If Menu only has one node, it's selected by default
		if (firstNode == lastNode && firstNode != NULL)
		{
			*selectedItemNumber = currentItem;
			selectedNode = firstNode;
		}
		while (currentNode != NULL && currentNode != lastNode)
		{
			menuItem = currentNode->getData();
			if (menuItem != NULL && menuItem->isSelected())
			{
				*selectedItemNumber = currentItem;
				selectedNode = currentNode;
				break;
			}

			currentItem++;
			currentNode = currentNode->getNext();
		}

		// Check last node
		if ((*selectedItemNumber) == -1 && currentNode != NULL && currentNode == lastNode)
		{
			menuItem = currentNode->getData();
			if (menuItem != NULL && menuItem->isSelected())
			{
				*selectedItemNumber = currentItem;
				selectedNode = currentNode;
			}
		}
	}

	return selectedNode;
}

/* */
int CGameMenu::getSelectedMenuItemNumber() const
{
	int selectedItemNumber = -1;

	if (m_MenuItemList != NULL)
	{
		getSelectedMenuNode(&selectedItemNumber);
	}

	return selectedItemNumber;
}

/* 
 * Select next or prev menu item. 
*/
void CGameMenu::selectMenuItem(bool next)
{
	if (m_MenuItemList != NULL)
	{
		int selectedItemNumber = -1;
		CCircularDoubleLinkedListNode<CGameMenuItem> *selectedNode = getSelectedMenuNode(&selectedItemNumber);

		// If we have a selected node, set it as unselected and select the next/prev one
		if (selectedNode != NULL)
		{
			CGameMenuItem *menuItem = selectedNode->getData();
			menuItem->setSelected(false);
			if (next)
			{
				selectedNode = selectedNode->getNext();
			}
			else
			{
				selectedNode = selectedNode->getPrev();
			}

			if (selectedNode != NULL)
			{
				menuItem = selectedNode->getData();
				if (menuItem != NULL)
				{
					menuItem->setSelected(true);
				}
			}
		}
	}
}

