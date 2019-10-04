#include "../stdafx.h"
#include "../Include/CLight.h"

/*
*/
template <class T>
T clampColor(T minimun, T maximum, T currValue)
{
	if (currValue < minimun)
	{
		return minimun;
	}
	if (currValue > maximum)
	{
		return maximum;
	}

	return currValue;
}

/*
 *
 */
CLight::CLight()
{
	m_color.X = DEFAULT_LIGHT_COLOR_RED;
	m_color.Y = DEFAULT_LIGHT_COLOR_GREEN;
	m_color.Z = DEFAULT_LIGHT_COLOR_BLUE;
	m_ambientIntensity = DEFAULT_AMBIENT_LIGHT_INTENSITY;
}

/*
 * Creates a light with float color values in the range 0..1, and ambient intensity in the range 0..1
 */
CLight::CLight(float red, float green, float blue, float ambientIntensity)
{
	red   = clampColor<float>(0.0f, 1.0f, red);
	green = clampColor<float>(0.0f, 1.0f, green);
	blue  = clampColor<float>(0.0f, 1.0f, blue);
	ambientIntensity = clampColor<float>(0.0f, 1.0f, ambientIntensity);

	m_color.X = red;
	m_color.Y = green;
	m_color.Z = blue;
	m_ambientIntensity = ambientIntensity;
}

/*
 * Creates a light with RGB color values in the range 0..256, and ambient intensity in the range 0..1
 */
CLight::CLight(int rgb_red, int rgb_green, int rgb_blue, float ambientIntensity)
{
	rgb_red   = clampColor<int>(0, 256, rgb_red);
	rgb_green = clampColor<int>(0, 256, rgb_green);
	rgb_blue  = clampColor<int>(0, 256, rgb_blue);
	ambientIntensity = clampColor<float>(0.0f, 1.0f, ambientIntensity);

	m_color.X = rgb_red   / 256.0f;
	m_color.Y = rgb_green / 256.0f;
	m_color.Z = rgb_blue  / 256.0f;
	m_ambientIntensity = ambientIntensity;
}

