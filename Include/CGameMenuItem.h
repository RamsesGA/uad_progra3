#pragma once

#ifndef CMENUITEM_H
#define CMENUITEM_H

#include "CColor.h"

// Class that represents a menu item
class CGameMenuItem
{
private:
	const char *m_Label;                 // Text on the menu
	float m_X, m_Y;                      // Screen coordinates (0.0 -> 1.0) of the position of this menu item
	CColor m_unselectedColor;            // Color of this menu item when unselected
	CColor m_selectedColor;              // Color of this menu item when selected
	bool m_Selected;                     // Is the menu currently selected?
	unsigned int m_arrayObjectID;        // Used for rendering

public:
	// Constructor and Destructor
	CGameMenuItem(const char * const menuLabel, float x, float y, unsigned int vaoId);
	~CGameMenuItem();

	// Get menu label
	const char * const getLabel() const { return m_Label; }

	// Get menu location
	void getLocation(float *x, float *y ) const ;

	// Get menu color
	const CColor * const getColor() const { return (m_Selected ? &m_selectedColor : &m_unselectedColor); }

	// Set menu color (RGB)
	void setColor(bool selected, unsigned short red, unsigned short green, unsigned short blue);

	// Get/Set for 'selected' flag 
	bool isSelected() const { return m_Selected; }
	void setSelected(bool selected) { m_Selected = selected; }

	// Get/Set for arrayObjectID
	unsigned int * getArrayObjectID() { return &m_arrayObjectID; }
	void setArrayObjectID(unsigned int id) { m_arrayObjectID = id; }
};

#endif // !CMENUITEM_H
