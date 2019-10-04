#include "../stdafx.h"

#include "../Include/CGameMenuItem.h"

/* */
CGameMenuItem::CGameMenuItem(const char * const menuLabel, float x, float y, unsigned int vaoId) :
	m_Label{ menuLabel },
	m_X{x},
	m_Y{y},
	m_Selected{false},
	m_arrayObjectID{vaoId}
{
	setColor(false, 255, 255, 255);  // Default unselected color: white
	setColor(true, 37, 134, 255);    // Default selected color:   blue
}

/* */
CGameMenuItem::~CGameMenuItem()
{
}

/* */
void CGameMenuItem::getLocation(float *x, float *y) const
{
	*x = m_X;
	*y = m_Y;
}

/* */
void CGameMenuItem::setColor(bool selected, unsigned short red, unsigned short green, unsigned short blue)
{
	if (selected)
	{
		m_selectedColor.setColorRGB(red, green, blue);
	}
	else
	{
		m_unselectedColor.setColorRGB(red, green, blue);
	}
}