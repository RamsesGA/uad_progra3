#pragma once

#ifndef CTEXTURE_LOADER_H
#define CTEXTURE_LOADER_H

#include "COpenGLRenderer.h"

class CTextureLoader
{
public:
	CTextureLoader() {}
	~CTextureLoader() {}

	// Helper method to load a TGA texture
	static bool loadTexture(const char *filename, unsigned int *newTextureID, COpenGLRenderer * const shp_OpenGLRenderer);
};

#endif // !CTEXTURE_LOADER_H
