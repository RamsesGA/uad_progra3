#pragma once

#ifndef CLIGHT_H
#define CLIGHT_H

#include "CVector3.h"

#define DEFAULT_LIGHT_COLOR_RED 1.0f
#define DEFAULT_LIGHT_COLOR_GREEN 1.0f
#define DEFAULT_LIGHT_COLOR_BLUE 1.0f
#define DEFAULT_AMBIENT_LIGHT_INTENSITY 1.0f

class CLight
{
private:
	CVector3 m_color;
	float m_ambientIntensity;

public:
	CLight();
	CLight(float red, float green, float blue, float ambientIntensity);
	CLight(int rgb_red, int rgb_green, int rgb_blue, float ambientIntensity);
	~CLight() {}

	CVector3 getColor() const { return m_color; }
	float getAmbientIntensity() const { return m_ambientIntensity; }
};

#endif // !CLIGHT_H

