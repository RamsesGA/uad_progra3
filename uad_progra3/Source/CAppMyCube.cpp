#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppMyCube.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"

/* */
CAppMyCube::CAppMyCube() : CAppMyCube(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppMyCube::CAppMyCube(int window_width, int window_height) : CApp(window_width, window_height), m_cubeShaderProgramID{ 0 }, m_cubeVertexArrayObject{ 0 }, m_cubeTextureID{ 0 }, m_numFaces{ 0 }
{
	cout << "Constructor: CAppMyCube(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppMyCube::~CAppMyCube()
{
	cout << "Destructor: ~CAppMyCube()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================

	// Free graphics memory used for the geometry
	if (m_cubeTextureID > 0 && getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->deleteTexture(&m_cubeTextureID);
	}

	// Free graphics memory used for the texture
	if (m_cubeVertexArrayObject > 0 && getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_cubeVertexArrayObject);
	}
}

/* */
void CAppMyCube::initialize()
{
	int    idx = 0;
	float  cubeSize = 2.0f;

	m_numFaces = 6 * 2; // 6 quads * 2 triangles per quad

	// SHADER
	// ----------------------------------------------------------------------------------------------------------------------------

	// Get shader for textured objects
	m_cubeShaderProgramID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_cubeShaderProgramID == 0)
	{
		cout << "ERROR: Unable to load shader for object" << endl;
		return;
	}

	// VERTICES
	// ----------------------------------------------------------------------------------------------------------------------------

	int    numVertices = 8;
	int    numVertexData = numVertices * 3;          // 8 vertices * 3 components (x, y, z) per vertex
	float *vertexData = new float[numVertexData]; // reserve memory for the vertex data

	// clear memory
	memset(vertexData, 0, sizeof(float) * numVertexData);

	vertexData[idx++] = -cubeSize / 2.0f; vertexData[idx++] = cubeSize; vertexData[idx++] = cubeSize / 2.0f; // FRONT TOP LEFT
	vertexData[idx++] = cubeSize / 2.0f; vertexData[idx++] = cubeSize; vertexData[idx++] = cubeSize / 2.0f; // FRONT TOP RIGHT
	vertexData[idx++] = -cubeSize / 2.0f; vertexData[idx++] = 0.0f;     vertexData[idx++] = cubeSize / 2.0f; // FRONT BOTTOM LEFT
	vertexData[idx++] = cubeSize / 2.0f; vertexData[idx++] = 0.0f;     vertexData[idx++] = cubeSize / 2.0f; // FRONT BOTTOM RIGHT

	vertexData[idx++] = -cubeSize / 2.0f; vertexData[idx++] = cubeSize; vertexData[idx++] = -cubeSize / 2.0f; // BACK TOP LEFT
	vertexData[idx++] = cubeSize / 2.0f; vertexData[idx++] = cubeSize; vertexData[idx++] = -cubeSize / 2.0f; // BACK TOP RIGHT
	vertexData[idx++] = -cubeSize / 2.0f; vertexData[idx++] = 0.0f;     vertexData[idx++] = -cubeSize / 2.0f; // BACK BOTTOM LEFT
	vertexData[idx++] = cubeSize / 2.0f; vertexData[idx++] = 0.0f;     vertexData[idx++] = -cubeSize / 2.0f; // BACK BOTTOM RIGHT

	// FACES
	// ----------------------------------------------------------------------------------------------------------------------------

	int             numFaceIndices = m_numFaces * 3;                     // 12 triangles * 3 vertex indices per face
	unsigned short *faceIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices

	// clear memory
	memset(faceIndices, 0, sizeof(unsigned short) * numFaceIndices);
	idx = 0;

	faceIndices[idx++] = 0; faceIndices[idx++] = 2; faceIndices[idx++] = 1; // FRONT #1
	faceIndices[idx++] = 1; faceIndices[idx++] = 2; faceIndices[idx++] = 3; // FRONT #2

	faceIndices[idx++] = 1; faceIndices[idx++] = 3; faceIndices[idx++] = 5; // RIGHT #1
	faceIndices[idx++] = 5; faceIndices[idx++] = 3; faceIndices[idx++] = 7; // RIGHT #2

	faceIndices[idx++] = 4; faceIndices[idx++] = 6; faceIndices[idx++] = 0; // LEFT #1
	faceIndices[idx++] = 0; faceIndices[idx++] = 6; faceIndices[idx++] = 2; // LEFT #2

	faceIndices[idx++] = 4; faceIndices[idx++] = 0; faceIndices[idx++] = 5; // TOP #1
	faceIndices[idx++] = 5; faceIndices[idx++] = 0; faceIndices[idx++] = 1; // TOP #2

	faceIndices[idx++] = 2; faceIndices[idx++] = 6; faceIndices[idx++] = 3; // BOTTOM #1
	faceIndices[idx++] = 3; faceIndices[idx++] = 6; faceIndices[idx++] = 7; // BOTTOM #2

	faceIndices[idx++] = 5; faceIndices[idx++] = 7; faceIndices[idx++] = 4; // BACK #1
	faceIndices[idx++] = 4; faceIndices[idx++] = 7; faceIndices[idx++] = 6; // BACK #2

	// NORMALS
	// ----------------------------------------------------------------------------------------------------------------------------

	int    numNormals = 6;
	int    numNormalData = numNormals * 3;        // 6 different normals * 3 components (x, y, z) each. 
												  // NOTE: we don't need two normals per quad (in this specific case). both triangles of a face use the same normal.
												  //       that's why we don't do numNormalData = numFaces * 3;
	float *normalData = new float[numNormalData]; // reserve memory for normal data

	// clear memory
	memset(normalData, 0, sizeof(float) * numNormalData);
	idx = 0;

	normalData[idx++] = 0.0f; normalData[idx++] = 0.0f; normalData[idx++] = 1.0f; // FRONT 
	normalData[idx++] = 1.0f; normalData[idx++] = 0.0f; normalData[idx++] = 0.0f; // RIGHT
	normalData[idx++] = -1.0f; normalData[idx++] = 0.0f; normalData[idx++] = 0.0f; // LEFT 
	normalData[idx++] = 0.0f; normalData[idx++] = 1.0f; normalData[idx++] = 0.0f; // TOP 
	normalData[idx++] = 0.0f; normalData[idx++] = -1.0f; normalData[idx++] = 0.0f; // BOTTOM 
	normalData[idx++] = 0.0f; normalData[idx++] = 0.0f; normalData[idx++] = -1.0f; // BACK

	// NORMAL INDICES
	// ----------------------------------------------------------------------------------------------------------------------------

	int numFaceNormalIndices = m_numFaces * 3;                                    // 12 triangles * 3 vertex indices per face
	unsigned short *faceNormalIndices = new unsigned short[numFaceNormalIndices]; // reserve memory for the face UV indices

	// clear memory
	memset(faceNormalIndices, 0, sizeof(unsigned short) * numFaceNormalIndices);
	idx = 0;

	faceNormalIndices[idx++] = 0; faceNormalIndices[idx++] = 0; faceNormalIndices[idx++] = 0; // FRONT #1
	faceNormalIndices[idx++] = 0; faceNormalIndices[idx++] = 0; faceNormalIndices[idx++] = 0; // FRONT #2

	faceNormalIndices[idx++] = 1; faceNormalIndices[idx++] = 1; faceNormalIndices[idx++] = 1; // RIGHT #1
	faceNormalIndices[idx++] = 1; faceNormalIndices[idx++] = 1; faceNormalIndices[idx++] = 1; // RIGHT #2

	faceNormalIndices[idx++] = 2; faceNormalIndices[idx++] = 2; faceNormalIndices[idx++] = 2; // LEFT #1
	faceNormalIndices[idx++] = 2; faceNormalIndices[idx++] = 2; faceNormalIndices[idx++] = 2; // LEFT #2

	faceNormalIndices[idx++] = 3; faceNormalIndices[idx++] = 3; faceNormalIndices[idx++] = 3; // TOP #1
	faceNormalIndices[idx++] = 3; faceNormalIndices[idx++] = 3; faceNormalIndices[idx++] = 3; // TOP #2

	faceNormalIndices[idx++] = 4; faceNormalIndices[idx++] = 4; faceNormalIndices[idx++] = 4; // BOTTOM #1
	faceNormalIndices[idx++] = 4; faceNormalIndices[idx++] = 4; faceNormalIndices[idx++] = 4; // BOTTOM #2

	faceNormalIndices[idx++] = 5; faceNormalIndices[idx++] = 5; faceNormalIndices[idx++] = 5; // BACK #1
	faceNormalIndices[idx++] = 5; faceNormalIndices[idx++] = 5; faceNormalIndices[idx++] = 5; // BACK #2


	// UV COORDS
	// ----------------------------------------------------------------------------------------------------------------------------

//	int    numVertexUVs    = 8;
//	int    numVertexUVData = numVertexUVs * 2;        // 8 vertices * 2 components (u,v) each vertex

	int    numVertexUVs = 14;                         // 14 vertices (8 vertices) + 6 duplicated vertices
	int    numVertexUVData = 14 * 2;                     // 14 vertices * 2 components (u,v) each vertex
	float *vertexUVData = new float[numVertexUVData]; // reserve memory for the face UV indices

	// clear memory
	memset(vertexUVData, 0, sizeof(float) * numVertexUVData);
	idx = 0;

	vertexUVData[idx++] = 0.25f; vertexUVData[idx++] = 0.33f; // 0:  TOP LEFT, FRONT
	vertexUVData[idx++] = 0.50f; vertexUVData[idx++] = 0.33f; // 1:  TOP RIGHT, FRONT
	vertexUVData[idx++] = 0.25f; vertexUVData[idx++] = 0.66f; // 2:  BOTTOM LEFT, FRONT
	vertexUVData[idx++] = 0.50f; vertexUVData[idx++] = 0.66f; // 3:  BOTTOM RIGHT, FRONT

	vertexUVData[idx++] = 0.25f; vertexUVData[idx++] = 0.00f; // 4:  TOP LEFT, BACK
	vertexUVData[idx++] = 0.50f; vertexUVData[idx++] = 0.00f; // 5:  TOP RIGHT, BACK
	vertexUVData[idx++] = 0.25f; vertexUVData[idx++] = 0.66f; // 6:  BOTTOM LEFT, BACK 	
	vertexUVData[idx++] = 0.50f; vertexUVData[idx++] = 0.66f; // 7:  BOTTOM RIGHT, BACK

	// DUPLICATE VERTICES (In UV space, not 3D space)
	vertexUVData[idx++] = 0.00f; vertexUVData[idx++] = 0.33f; // 8:  TOP LEFT, BACK
	vertexUVData[idx++] = 0.00f; vertexUVData[idx++] = 0.66f; // 9:  BOTTOM LEFT, BACK
	vertexUVData[idx++] = 0.75f; vertexUVData[idx++] = 0.33f; // 10: TOP RIGHT, BACK
	vertexUVData[idx++] = 0.75f; vertexUVData[idx++] = 0.66f; // 11: BOTTOM RIGHT, BACK
	vertexUVData[idx++] = 1.00f; vertexUVData[idx++] = 0.33f; // 12: TOP LEFT, BACK
	vertexUVData[idx++] = 1.00f; vertexUVData[idx++] = 0.66f; // 13: BOTTOM LEFT, BACK

	// UV COORDS INDICES
	// ----------------------------------------------------------------------------------------------------------------------------

	int             numFaceUVIndices = m_numFaces * 3;                       // 12 triangles * 3 vertex indices per face
	unsigned short *faceUVIndices = new unsigned short[numFaceUVIndices]; // reserve memory for the face UV indices

	// clear memory
	memset(faceUVIndices, 0, sizeof(unsigned short) * numFaceUVIndices);
	idx = 0;

	/*
	Original indices that result in the texture to be placed incorrectly in some faces (i.e. RIGHT, LEFT, BACK)
	╔═════════════════════════╗
	║     4------5            ║
	║     |      |            ║
	║     0------1            ║
	║     |      |            ║
	║     |      |            ║
	║     2------3            ║
	║     |      |            ║
	║     6------7            ║
	║                         ║
	╚═════════════════════════╝
	*/
	/*
	faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 2;  faceUVIndices[idx++] = 1;  // FRONT #1
	faceUVIndices[idx++] = 1;  faceUVIndices[idx++] = 2;  faceUVIndices[idx++] = 3;  // FRONT #2

	faceUVIndices[idx++] = 1;  faceUVIndices[idx++] = 3;  faceUVIndices[idx++] = 5;  // RIGHT #1
	faceUVIndices[idx++] = 5;  faceUVIndices[idx++] = 3;  faceUVIndices[idx++] = 7;  // RIGHT #2

	faceUVIndices[idx++] = 4;  faceUVIndices[idx++] = 6;  faceUVIndices[idx++] = 0;  // LEFT #1
	faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 6;  faceUVIndices[idx++] = 2;  // LEFT #2

	faceUVIndices[idx++] = 4;  faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 5;  // TOP #1
	faceUVIndices[idx++] = 5;  faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 1;  // TOP #2

	faceUVIndices[idx++] = 2;  faceUVIndices[idx++] = 6; faceUVIndices[idx++] = 3;  // BOTTOM #1
	faceUVIndices[idx++] = 3;  faceUVIndices[idx++] = 6; faceUVIndices[idx++] = 7;  // BOTTOM #2

	faceUVIndices[idx++] = 5;  faceUVIndices[idx++] = 7; faceUVIndices[idx++] = 4;  // BACK #1
	faceUVIndices[idx++] = 4;  faceUVIndices[idx++] = 7; faceUVIndices[idx++] = 6;  // BACK #2
	*/



	/*
	NEW indices

	╔═════════════════════════╗
	║     4------5            ║
	║     |  T   |            ║  F: FRONT
	║8----0------1----10----12║  R: RIGHT
	║|    |      |     |     |║  L: LEFT
	║| L  |  F   |  R  | BK  |║  T: TOP
	║9----2------3----11----13║  B: BOTTOM
	║     |  B   |            ║  BK: BACK
	║     6------7            ║
	║                         ║
	╚═════════════════════════╝

	Indices:
		5 and 10
		8, 4, and 12
		7 and 11
		6, 9, and 13
	Refer to the same vertex in 3d space, but in 2D we need to duplicate them to map them properly
	 */

	faceUVIndices[idx++] = 0;   faceUVIndices[idx++] = 2;  faceUVIndices[idx++] = 1;   // FRONT #1
	faceUVIndices[idx++] = 1;   faceUVIndices[idx++] = 2;  faceUVIndices[idx++] = 3;   // FRONT #2

	faceUVIndices[idx++] = 1;   faceUVIndices[idx++] = 3;  faceUVIndices[idx++] = 10;  // RIGHT #1
	faceUVIndices[idx++] = 10;  faceUVIndices[idx++] = 3;  faceUVIndices[idx++] = 11;  // RIGHT #2

	faceUVIndices[idx++] = 8;   faceUVIndices[idx++] = 9;  faceUVIndices[idx++] = 0;   // LEFT #1
	faceUVIndices[idx++] = 0;   faceUVIndices[idx++] = 9;  faceUVIndices[idx++] = 2;   // LEFT #2

	faceUVIndices[idx++] = 4;   faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 5;   // TOP #1
	faceUVIndices[idx++] = 5;   faceUVIndices[idx++] = 0;  faceUVIndices[idx++] = 1;   // TOP #2

	faceUVIndices[idx++] = 2;   faceUVIndices[idx++] = 6; faceUVIndices[idx++] = 3;    // BOTTOM #1
	faceUVIndices[idx++] = 3;   faceUVIndices[idx++] = 6; faceUVIndices[idx++] = 7;    // BOTTOM #2

	faceUVIndices[idx++] = 10;  faceUVIndices[idx++] = 11; faceUVIndices[idx++] = 12;  // BACK #1
	faceUVIndices[idx++] = 12;  faceUVIndices[idx++] = 11; faceUVIndices[idx++] = 13;  // BACK #2


	// SAVE GEOMETRY
	// ----------------------------------------------------------------------------------------------------------------------------

	// Allocate graphics memory for object
	bool loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_cubeShaderProgramID,   // Shader to use for this object
		&m_cubeVertexArrayObject, // NEW ID that is going to be created when saving this geometry to graphics memory
		vertexData,               // Vertices
		numVertices,              // Number of vertices (internal code multiplies sizeof(float) * numVertices * 3)
		normalData,               // Normals
		numNormals,               // Number of normals, (internal code multiplies sizeof(float) * numNormals * 3)
		vertexUVData,             // Texture coordinates
		numVertexUVs,             // Number of tex. coords. (internal code multiplies sizeof(float) * numVertexUVs * 2)
		faceIndices,              // Triangle indices
		numFaceIndices / 3,       // Number of tri. indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
		faceNormalIndices,        // Normal indices
		numFaceNormalIndices / 3, // Number of normal indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
		faceUVIndices,            // Texture coordinates indices
		numFaceUVIndices / 3      // Number of tex. coord. indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
	);

	if (!loadedToGraphicsCard)
	{
		cout << "ERROR: Unable to save geometry to graphics card" << endl;

		m_numFaces = 0;

		if (m_cubeVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_cubeVertexArrayObject);
			m_cubeVertexArrayObject = 0;
		}
	}
	else
	{
		std::wstring wresourceFilenameTexture;
		std::string resourceFilenameTexture;

		// Check texture file exists for the textured cube
		if (CWideStringHelper::GetResourceFullPath(MC_GRASS_3_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
		{
			// Load texture
			m_cubeTextureID = 0;
			if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &m_cubeTextureID, getOpenGLRenderer()))
			{
				cout << "ERROR: Unable load texture for textured cube" << endl;
				m_cubeTextureID = 0;
			}
		}
		else
		{
			cout << "ERROR: Unable to find one or more resources: " << endl;
			cout << "  " << MC_GRASS_3_TEXTURE << endl;
		}
	}

	// FREE MEMORY
	// ----------------------------------------------------------------------------------------------------------------------------

	delete[] vertexData;
	delete[] faceIndices;
	delete[] normalData;
	delete[] faceNormalIndices;
	delete[] vertexUVData;
	delete[] faceUVIndices;
}

/* */
void CAppMyCube::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_CUBE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.55f, 0.60f, 0.25f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppMyCube::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// The code to "animate" anything object (change position, scale, etc)
	// should be done in this method.
}

/* */
void CAppMyCube::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		if (m_numFaces > 0 && m_cubeVertexArrayObject > 0 && m_cubeShaderProgramID > 0)
		{
			// White 
			// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
			float color[3] = { 1.0f, 1.0f, 1.0f };

			// Position object in (0, 0, 0)
			CVector3 pos = CVector3::ZeroVector();

			// Angle in RADIANS! (multiply * PI / 180.0f to convert to radians)
			float rotationAroundY = 125.0f * PI_OVER_180;

			// Create a simple model matrix, with given position and rotation of zero degrees around Y axis
			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(rotationAroundY, pos);

			// Render the cube
			getOpenGLRenderer()->renderObject(
				&m_cubeShaderProgramID,                      // Shader to use to render the object
				&m_cubeVertexArrayObject,                    // ID for the geometry
				&m_cubeTextureID,                            // ID for the texture
				m_numFaces,                                  // Number of triangles that the object has
				color,                                       // Color (tint)
				&modelMatrix,                                // Object transformation
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES, //
				false                                        //
			);
		}
	}
}

/* */
void CAppMyCube::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppMyCube::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}