#include "../stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "../Include/CAppGeometricFigures.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"

/* */
CAppGeometricFigures::CAppGeometricFigures() :
	CAppGeometricFigures(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppGeometricFigures::CAppGeometricFigures(int window_width, int window_height) : CApp(window_width, window_height), m_currentDeltaTime{ 0.0 }, m_objectRotation{ 0.0 }, m_objectPosition{ -1.5f, 0.0f, 0.0f }, m_rotationSpeed{ DEFAULT_ROTATION_SPEED }, m_pyramidVertexArrayObject{ 0 }, m_numFacesPyramid{ 0 }, m_renderPolygonMode{ 0 }	
{
	cout << "Constructor: CAppGeometricFigures(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppGeometricFigures::~CAppGeometricFigures()
{
	cout << "Destructor: ~CAppGeometricFigures()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_pyramidVertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
	}
	// =================================================
}

/* */
void CAppGeometricFigures::initialize()
{
	// Initialize app-specific stuff here
	// Get shader for color objects
	m_colorModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_colorModelShaderId == 0)
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}
	// Get shader for textured objects
	m_texturedModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_texturedModelShaderId == 0)
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_LEAVES_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	// Initialize the texture
	m_textureID = 0;
	if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &m_textureID, getOpenGLRenderer()))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	m_initialized = true;

	CVector3 center(0, 0, 0);
	createHexaGemoetry(center, 3, 1, false);
	structureHexa();

	//createPyramidGeometry();
}

/* */
void CAppGeometricFigures::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
		{
			// ALWAYS call initialize AFTER creating the window
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();

				// Enter main loop
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* */
void CAppGeometricFigures::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	double degreesToRotate = 0.0;

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
}

/* *///IMPORTANTE
void CAppGeometricFigures::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL && menu->isInitialized() && menu->isActive())
	{
	}
	else // Otherwise, render app-specific stuff here...
	{
		// White , Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;
	
		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_objectPosition);

		//if (m_pyramidVertexArrayObject > 0 && m_numFacesPyramid > 0)
		//{
		//	CVector3 pos2 = m_objectPosition;
		//	pos2 += CVector3(3.0f, 0.0f, 0.0f);
		//	MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, pos2);
		//
		//	// Render pyramid in the first position, using the color shader
		//	getOpenGLRenderer()->renderObject(
		//		&m_colorModelShaderId,
		//		&m_pyramidVertexArrayObject,
		//		&noTexture,
		//		m_numFacesPyramid,
		//		color,
		//		&modelMatrix,
		//		COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
		//		false
		//	);

		if (m_pyramidVertexArrayObject > 0 && m_caras_hexa > 0)
		{
			CVector3 pos2 = m_objectPosition;
			pos2 += CVector3(3.0f, 0.0f, 0.0f);
			MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, CVector3(0.0, 0.0, 0.0));

			// Render same pyramid (same vertex array object identifier), in a second position, but this time with a texture
			getOpenGLRenderer()->renderObject(
				&m_texturedModelShaderId,
				&m_pyramidVertexArrayObject,
				&m_textureID,
				m_caras_hexa,
				color,
				&modelMatrix2,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}

		// =================================
	}
}

/* */
void CAppGeometricFigures::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

//Constructor de puntos del Hexa
CVector3 CAppGeometricFigures::createHexaGemoetry(CVector3 _center, int _indice, float _size, bool _p)
{
	CVector3 point;
	float pi = 3.1415926535f;
	bool loaded = false;


	float angle_deg = (60 * _indice) - 30;
	float angle_rad = (pi / 180) * angle_deg;

	point.Y = 0.0f;
	point.X = _center.X + (_size * cos(angle_rad));
	point.Z = _center.Y + (_size * sin(angle_rad));

	return point;
}

void CAppGeometricFigures::structureHexa()
{
	bool carga = false;

	CVector3 p_1, p_2, p_3, p_4, p_5, p_6;
	CVector3 v1, v2, v3, v1v2, v1v3, norm;

	p_1 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 1, 3, false);
	p_2 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 2, 3, false);
	p_3 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 3, 3, false);
	p_4 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 4, 3, false);
	p_5 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 5, 3, false);
	p_6 = createHexaGemoetry(CVector3(0.0f, 0.0f, 0.0f), 6, 3, false);

	//--------------------
	//numero de vertices * 3
	float vertex_data[18] = { 
	 p_1.X, p_1.Y, p_1.Z,
	 p_2.X, p_2.Y, p_2.Z ,
	 p_3.X, p_3.Y, p_3.Z ,
	 p_4.X, p_4.Y, p_4.Z ,
	 p_5.X, p_5.Y, p_5.Z ,
	 p_6.X, p_6.Y, p_6.Z };

	//numero de vertices *2
	float vertex_UVs[12] = {0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5};

	//numero de caras y cada cara tiene 3 componentes (4*3 = 12)
	unsigned short face_Indices[12] = {
	0,1,2,
	2,3,5,
	3,4,5,
	5,0,2
	};

	//numero de caras * sus 3 componentes (4*3 = 12)
	float normal_Data[12] = { 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 };

	unsigned short face_Normal_Indices[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };

	m_caras_hexa = 4;

	for (int i = 0; i < m_caras_hexa; i++)
	{
		// Vertex 1
		v1.setValues(
			vertex_data[face_Indices[i * 3]],
			vertex_data[face_Indices[i * 3] + 1],
			vertex_data[face_Indices[i * 3] + 2]);

		// Vertex 2
		v2.setValues(
			vertex_data[face_Indices[(i * 3) + 1]],
			vertex_data[face_Indices[(i * 3) + 1] + 1],
			vertex_data[face_Indices[(i * 3) + 1] + 2]
		);

		// Vertex 3
		v3.setValues(
			vertex_data[face_Indices[(i * 3) + 2]],
			vertex_data[face_Indices[(i * 3) + 2] + 1],
			vertex_data[face_Indices[(i * 3) + 2] + 2]
		);

		// Vector from v1 to v2
		v1v2 = v2 - v1;

		// Vector from v1 to v3
		v1v3 = v3 - v1;

		norm = v1v2.cross(v1v3);
		norm.normalize();

		normal_Data[i * 3] = norm.X;
		normal_Data[(i * 3) + 1] = norm.Y;
		normal_Data[(i * 3) + 2] = norm.Z;
	}

	// Allocate graphics memory for object
	carga = getOpenGLRenderer()->allocateGraphicsMemoryForObject(&m_colorModelShaderId, &m_pyramidVertexArrayObject, vertex_data,
		6,	// Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		normal_Data,
		4,
		vertex_UVs,
		6,
		face_Indices,
		4,	// Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		face_Normal_Indices,
		4,
		face_Indices,
		4
	);

	if (!carga)
	{
		m_caras_hexa = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}

}

/* */
void CAppGeometricFigures::createPyramidGeometry()
{
	float height = 2.25f;
	float sideHalfX = 0.75f;
	float sideHalfZ = 1.0f;
	bool loaded = false;
	CVector3 v1, v2, v3, v1v2, v1v3, norm;

	float vertexData[15] = {
		0.0, height, 0.0,                // TOP
		-sideHalfX,  0.0,   sideHalfZ,   // BOTTOM LEFT, FRONT
	 	 sideHalfX,  0.0,   sideHalfZ,   // BOTTOM RIGHT, FRONT
		-sideHalfX,  0.0,  -sideHalfZ,   // BOTTOM LEFT, BACK
		 sideHalfX,  0.0,  -sideHalfZ    // BOTTOM RIGHT, BACK
	};

	float vertexUVs[10] =
	{
		0.5f,  0.11f, // TOP 
		0.25f, 0.99f, // BOTTOM LEFT, FRONT
		0.75f, 0.99f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f  // BOTTOM RIGHT, BACK 
	};

	m_numFacesPyramid = 6;

	unsigned short faceIndices[18] = {
		0, 1, 2,    
		0, 2, 4,    
		0, 4, 3,    
		0, 3, 1,    
		1, 3, 2,
		2, 3, 4
	};

	float normalData[18] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};

	unsigned short faceNormalIndices[18] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3,
		4, 4, 4,
		5, 5, 5
	};

	for (int i = 0; i < m_numFacesPyramid; i++)
	{
		// Vertex 1
		v1.setValues(
			vertexData[faceIndices[i * 3]],
			vertexData[faceIndices[i * 3] + 1],
			vertexData[faceIndices[i * 3] + 2]);

		// Vertex 2
		v2.setValues(
			vertexData[faceIndices[(i * 3) + 1]],
			vertexData[faceIndices[(i * 3) + 1] + 1],
			vertexData[faceIndices[(i * 3) + 1] + 2]
		);

		// Vertex 3
		v3.setValues(
			vertexData[faceIndices[(i * 3) + 2]],
			vertexData[faceIndices[(i * 3) + 2] + 1],
			vertexData[faceIndices[(i * 3) + 2] + 2]
		);

		// Vector from v1 to v2
		v1v2 = v2 - v1;

		// Vector from v1 to v3
		v1v3 = v3 - v1;

		norm = v1v2.cross(v1v3);
		norm.normalize();

		normalData[i * 3] = norm.X;
		normalData[(i * 3) + 1] = norm.Y;
		normalData[(i * 3) + 2] = norm.Z;
	}

	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(&m_colorModelShaderId, &m_pyramidVertexArrayObject, vertexData,
		5,	// Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		normalData,
		6,
		vertexUVs,
		5,
		faceIndices,
		6,	// Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		faceNormalIndices,
		6,
		faceIndices,
		6
	);

	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
}

/* */
void CAppGeometricFigures::onF2(int mods)
{
}

/* */
void CAppGeometricFigures::onF3(int mods)
{
	if (m_renderPolygonMode == 0)
	{
		getOpenGLRenderer()->setFillPolygonMode();
		m_renderPolygonMode = 1;
	}
	else
	{
		getOpenGLRenderer()->setWireframePolygonMode();
		m_renderPolygonMode = 0;
	}
}
