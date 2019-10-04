#pragma once

#ifndef CGAMEMENU_H
#define CGAMEMENU_H

#include "CCircularDoubleLinkedList.h"
#include "CGameMenuItem.h"
#include "COpenGLRenderer.h"

#define DEFAULT_MENU_ITEM_WIDTH 0.5f
#define DEFAULT_MENU_ITEM_HEIGHT 0.1f

// Class that represents the app menu
// The menu item elements are actually nodes of a double linked list (may be overkill, but they serve the purpose of practicing data structures)
// When the user presses the key up/down (captured in CGameWindow), the appropriate method in this class is called to select the next/prev menu item.
// In this case, "selecting" a menu item means using a different color to render it, and when the user presses the Enter key, the associated action is executed
// This associated action is defined in the CApp derived class.
// Both the Double-Linked list and the nodes are templates. The nodes hold CMenuItem objects.
class CGameMenu
{
private:
	bool m_Active;      // Is menu active?
	bool m_Initialized; // Is menu initialized?

	unsigned int m_shaderProgramId;
	unsigned int m_textureObjectId;

	// The menu is a double-linked list of CMenuItem objects
	CCircularDoubleLinkedList<CGameMenuItem> *m_MenuItemList;
	float m_MenuItemWidth;
	float m_MenuItemHeight;

	CCircularDoubleLinkedListNode<CGameMenuItem> * getSelectedMenuNode(int *selectedItemNumber) const;

public:
	CGameMenu();
	~CGameMenu();

	// Setter/getter for m_Active
	bool isActive() const { return m_Active; }
	void setActive(bool active) { m_Active = active; }

	// Getter for m_Initialized
	bool isInitialized() const { return m_Initialized; }

	// Set menu item width/height
	void setMenuItemWidth(float width) { m_MenuItemWidth = width; }
	void setMenuItemHeight(float height) { m_MenuItemHeight = height; }

	// Add menu item
	void addMenuItem(const char * const menuLabel, float x, float y, unsigned int vaoId/*, CMenuItem *parent*/);

	// Render
	void render(COpenGLRenderer * const renderer);

	// Cleanup graphics objects
	void cleanupGraphicsObjects(COpenGLRenderer * const renderer);

	// Get the number of the selected menu item
	int getSelectedMenuItemNumber() const;
	
	// Select next/prev menu item
	void selectMenuItem(bool next);

	//
	void setShaderProgramId(unsigned int id) { m_shaderProgramId = id; }
	void setTextureObjectId(unsigned int id) { m_textureObjectId = id; }
};

#endif // !CGAMEMENU_H
