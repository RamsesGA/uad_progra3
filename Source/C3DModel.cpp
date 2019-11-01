#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/C3DModel.h"
#include "../Include/C3DModel_Obj.h"
#include "../Include/C3DModel_FBX.h"
#include "../Include/CTextureLoader.h"

/* */
C3DModel::C3DModel()
	: m_vertexIndices(NULL),
	m_normalIndices(NULL),
	m_UVindices(NULL),
	m_verticesRaw(NULL),
	m_normalsRaw(NULL),
	m_uvCoordsRaw(NULL),
	m_modelGeometryLoaded(false),
	m_numVertices(0), 
	m_numNormals(0), 
	m_numUVCoords(0), 
	m_numFaces(0),
	m_graphicsMemoryObjectId(0),
	m_shaderProgramId(0),
	m_textureObjectId(0),
	m_modelHasNormals(false),
	m_modelHasUVs(false),
	m_modelHasTextures(false),
	m_modelTextureFilename(nullptr)
{
	cout << "Constructor: C3DModel()" << endl;
}

/* */
C3DModel::~C3DModel()
{
	cout << "Destructor: C3DModel()" << endl;
	reset();
}

/*
*/
void C3DModel::reset()
{
	if (m_vertexIndices != NULL)
	{
		cout << "deleting vertex indices" << endl;
		delete[] m_vertexIndices;
		m_vertexIndices = NULL;
	}
	if (m_normalIndices != NULL)
	{
		cout << "deleting normal indices" << endl;
		delete[] m_normalIndices;
		m_normalIndices = NULL;
	}
	if (m_UVindices != NULL)
	{
		cout << "deleting UV indices" << endl;
		delete[] m_UVindices;
		m_UVindices = NULL;
	}
	if (m_verticesRaw != NULL)
	{
		cout << "deleting vertices (float)" << endl;
		delete[] m_verticesRaw;
		m_verticesRaw = NULL;
	}
	if (m_normalsRaw != NULL)
	{
		cout << "deleting normals (float)" << endl;
		delete[] m_normalsRaw;
		m_normalsRaw = NULL;
	}
	if (m_uvCoordsRaw != NULL)
	{
		cout << "deleting uvCoords (float)" << endl;
		delete[] m_uvCoordsRaw;
		m_uvCoordsRaw = NULL;
	}
	if (m_modelTextureFilename != nullptr)
	{
		delete[] m_modelTextureFilename;
		m_modelTextureFilename = nullptr;
	}

	m_materialNames.clear();
	m_materialFilenames.clear();
	m_materialColors.clear();
	
	m_numVertices = 0;
	m_numNormals = 0;
	m_numUVCoords = 0;
	m_numFaces = 0;
	
	m_modelGeometryLoaded = false;

	m_graphicsMemoryObjectId = 0;
	m_shaderProgramId = 0;
	m_textureObjectId = 0;
	m_modelHasNormals = false;
	m_modelHasUVs = false;
	m_modelHasTextures = false;
}

/*
 Static method ("abstract method" pattern) that checks the filename and returns a new object of the appropriate subclass
*/
C3DModel* C3DModel::load(const char * const filename, COpenGLRenderer * const shp_OpenGLRenderer)
{
	C3DModel *newModel = nullptr;

	// Check the file type
	// We could use the "PathFindExtension" function but that needs the shlwapi.lib, instead we'll keep it simple and avoid more dependencies
	string stdFilename(filename);
	
	size_t dotIndex = stdFilename.rfind('.', stdFilename.length());
	
	if (dotIndex != string::npos)
	{
		string fileExtension = stdFilename.substr(dotIndex + 1, stdFilename.length() - dotIndex);

		// Convert to lowercase
		// NOTE: ::tolower works on single bytes, which can be a problem for multi-byte encoding, like UTF8
		transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

		cout << "File extension: " << fileExtension << endl;

		// Now check the file type and see if it's a supported type
		if (!fileExtension.compare("obj"))
		{
			cout << "Loading OBJ model..." << endl;
			newModel = new C3DModel_Obj();
			
			if (!newModel->loadFromFile(filename))
			{
				newModel->reset();
			}
		}
		else if (!fileExtension.compare("fbx"))
		{
			//ESCENCIAL PARA EL PROYECTO
			cout << "\nCargando modelo FBX" << endl;
			newModel = new C3DModel_FBX();

			if (!newModel->loadFromFile(filename))
			{
				newModel->reset();
			}
		}
		else if (!fileExtension.compare("3ds"))
		{
			cout << "3DS file format reading not implemented" << endl;
		}
		else if (!fileExtension.compare("stl"))
		{
			cout << "STL file format reading not implemented" << endl;
		}
		
		
		// If model geometry could be loaded, save it to graphics card
		if (newModel != nullptr && newModel->isGeometryLoaded() && shp_OpenGLRenderer != nullptr)
		{
			unsigned int newModelShaderId = 0;
			unsigned int newTextureID = 0;
			unsigned int newModelVertexArrayObject = NULL;
			bool loadedToGraphicsCard = false;

			// By default, shaders to be loaded are for non-textured objects, but if the model has a valid texture filename and UVs, 
			// load the appropriate shader instead 
			if (newModel->hasUVs() && newModel->hasTextures())
			{
				// Switch shaders to textured object ones
				newModelShaderId = shp_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

				// LOAD TEXTURE AND ALSO CREATE TEXTURE OBJECT
				if (CTextureLoader::loadTexture(newModel->getTextureFilename(), &newTextureID, shp_OpenGLRenderer))
				{
					newModel->setTextureObjectId(newTextureID);
				}
				else
				{
					// Texture could not be loaded, default back to color shader
					newModel->setTextureObjectId(0);
					newModelShaderId = shp_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
				}
			}
			else
			{
				// Load color shader
				newModelShaderId = shp_OpenGLRenderer->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
			}

			// Save the shader program ID in the model
			newModel->setShaderProgramId(newModelShaderId);

			// Return if shaders are not loaded
			if (newModelShaderId == 0)
			{
				cout << "ERROR: Unable to load shaders for 3D object." << endl;
			}
			else
			{
				// Allocate graphics memory for object
				loadedToGraphicsCard = shp_OpenGLRenderer->allocateGraphicsMemoryForObject(
					&newModelShaderId,
					&newModelVertexArrayObject,
					newModel->getModelVertices(),
					newModel->getNumVertices(),
					newModel->getModelNormals(),
					newModel->getNumNormals(),
					newModel->getModelUVCoords(),
					newModel->getNumUVCoords(),
					newModel->getModelVertexIndices(),
					newModel->getNumFaces(),
					newModel->getModelUVCoordIndices(),
					((newModel) ? (newModel->getNumFaces()) : 0),
					newModel->getModelUVCoordIndices(),
					((newModel) ? (newModel->getNumFaces()) : 0)
				);

				newModel->setGraphicsMemoryObjectId(newModelVertexArrayObject);
				


				// If error ocurred, cleanup memory
				if (!loadedToGraphicsCard)
				{
					newModel->setGraphicsMemoryObjectId(0);
				}
			}
		}
	}
	else
	{
		cout << "ERROR: Cannot determine the file type" << endl;
		return nullptr;
	}

	return newModel;
}

/*
*/
void C3DModel::computeFaceNormals()
{
	// Calculate face normals
	// ...

	// After calculating the normals, set the flag to true
	// m_modelHasNormals = true;
}