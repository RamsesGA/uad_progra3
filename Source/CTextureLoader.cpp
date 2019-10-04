#include "../stdafx.h"
#include "../Include/CTextureLoader.h"
#include "../Include/LoadTGA.h"

/*
*/
bool CTextureLoader::loadTexture(const char *filename, unsigned int *newTextureID, COpenGLRenderer * const shp_OpenGLRenderer)
{
	TGAFILE tgaFile;
	tgaFile.imageData = nullptr;

	if (filename == nullptr || newTextureID == nullptr || shp_OpenGLRenderer == nullptr)
	{
		return false;
	}

	*newTextureID = 0;

	if (LoadTGAFile(filename, &tgaFile))
	{
		if (tgaFile.imageData == nullptr ||
			tgaFile.imageHeight < 0 ||
			tgaFile.imageWidth < 0)
		{
			if (tgaFile.imageData != nullptr)
			{
				delete[] tgaFile.imageData;
			}

			return false;
		}

		// Create a texture object for the menu, and copy the texture data to graphics memory
		if (!shp_OpenGLRenderer->createTextureObject(
			newTextureID,
			tgaFile.imageData,
			tgaFile.imageWidth,
			tgaFile.imageHeight
		))
		{
			return false;
		}

		// Texture data is stored in graphics memory now, we don't need this copy anymore
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}
	}
	else
	{
		// Free texture data
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}

		return false;
	}

	return true;
}
