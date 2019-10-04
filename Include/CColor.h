#pragma once

#ifndef _CCOLOR_H
#define _CCOLOR_H

// Simple C++ class to practice class creation
class CColor
{
private:
	float m_Red, m_Green, m_Blue;

public:
	//
	CColor() :
		m_Red(0.0f),
		m_Green(0.0f),
		m_Blue(0.0f)
	{}

	//
	CColor(float red, float green, float blue)
	{
		setColor(red, green, blue);
	}

	//
	CColor(unsigned short red, unsigned short green, unsigned short blue)
	{
		setColor(red, green, blue);
	}
	
	//
	~CColor() {}

	// Note: Caller must make sure the pointers are valid
	void getColor(float *red, float *green, float *blue) const 
	{ 
		*red   = m_Red; 
		*green = m_Green; 
		*blue  = m_Blue; 
	}

	// Note: Caller must make sure the pointer is valid
	void getColorfv(float *color) const
	{
		color[0] = m_Red;
		color[1] = m_Green;
		color[2] = m_Blue;
	}

	// Note: Caller must make sure the pointers are valid
	void getColorRGB(unsigned short *red, unsigned short *green, unsigned short *blue) const 
	{
		*red   = (unsigned short)(m_Red   * 255.0f);
		*green = (unsigned short)(m_Green * 255.0f);
		*blue  = (unsigned short)(m_Blue  * 255.0f);
	}

	// 
	void setColor(float red, float green, float blue) 
	{
		float MINF = 0.0f;
		float MAXF = 1.0f;

		// Clamp values in the range 0.0 .. 1.0
		m_Red   = red   <= MINF ? MINF : red   >= MAXF ? MAXF : red;
		m_Green = green <= MINF ? MINF : green >= MAXF ? MAXF : green;
		m_Blue  = blue  <= MINF ? MINF : blue  >= MAXF ? MAXF : blue;
	}

	//
	void setColorRGB(unsigned short red, unsigned short green, unsigned short blue) 
	{
		unsigned short MINRGB = 0;
		unsigned short MAXRGB = 255;

		// Clamp values in the range 0..255
		red   = red   <= MINRGB ? MINRGB : red   >= MAXRGB ? MAXRGB : red;
		green = green <= MINRGB ? MINRGB : green >= MAXRGB ? MAXRGB : green;
		blue  = blue  <= MINRGB ? MINRGB : blue  >= MAXRGB ? MAXRGB : blue;

		// Convert to range 0.0 .. 1.0
		m_Red   = red   / 255.0f;
		m_Green = green / 255.0f;
		m_Blue  = blue  / 255.0f;
	}
};

#endif // !_CCOLOR_H
