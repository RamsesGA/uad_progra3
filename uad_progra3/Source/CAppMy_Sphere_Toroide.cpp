#include "../stdafx.h"

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppMy_Sphere_Toroide.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"

/* */
CAppMy_Sphere_Toroide::CAppMy_Sphere_Toroide() : CAppMy_Sphere_Toroide(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppMy_Sphere_Toroide::CAppMy_Sphere_Toroide(int window_width, int window_height) : CApp(window_width, window_height)
{
	cout << "Constructor: CAppMy_Sphere_Toroide(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppMy_Sphere_Toroide::~CAppMy_Sphere_Toroide()
{
	cout << "Destructor: ~CAppMy_Sphere_Toroide()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
}

void CAppMy_Sphere_Toroide::CreateSphere()
{
	vector	<float> indices;
	int LH = 5;
	int LV =5;
	int radio = 2;
	float grado1 = 180 / (LH + 1);
	float grado2 = 180 / (LV);

	m_numFaces = (2 * (LV * 2))*LH;
	int numVertex = ((LV * 2)*LH) + 2;
	int numVerteData = numVertex * 3;
	float *vertexData = new float[numVerteData];

	int idx = 0;
	int numIdx = 0;
	const float pi = 3.1415926535f;
	for (float i = 90; i >=-90; i-=grado1)
	{
		if (i==90||i<=-90)
		{
			vertexData[idx++] = radio * cos(i*(pi/180))*cos(0);
			vertexData[idx++] = radio * sin(i*(pi / 180));
			vertexData[idx++] = radio * cos(i*(pi / 180))*sin(0);
			indices.push_back(numIdx);
			numIdx += 1;
		}
		else
		{
			for (float j = 0; j < 360; j+=grado2)
			{
				vertexData[idx++] = radio * cos(i*(pi/180))*cos(j*(pi/180));
				vertexData[idx++] = radio * sin(i*(pi/180));
				vertexData[idx++] = radio * cos(i*(pi/180))*sin(j*(pi/180));
				indices.push_back(numIdx);
				numIdx += 1;
			}
		}
	}

	int             numFaceIndices = m_numFaces * 3;                     // x triangles * 3 vertex indices per face
	unsigned short *faceIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices
	memset(faceIndices, 0, sizeof(unsigned short) * numFaceIndices);
	int inidx = 0;
	int numHori = 1;
	for (int i = 0; i < numIdx-1; i++)
	{
		if (i>= numIdx - 1)
		{
			break;
		}
		if (i == 0) 
		{
			for (int j = 1; j < LV*2+1; j++)
			{
				faceIndices[inidx++] = 0;
				faceIndices[inidx++] = j;
				if (j == LV * 2)
				{
					faceIndices[inidx++] = 1;
				}
				else
				{
					faceIndices[inidx++] = j +1;
				}
			}
		}
		else if(i == ((LV * 2)* (LH-1))+1)
		{
			for (int j = ((LV * 2)* (LH - 1)) + 1; j < (LV * 2 * LH)+1; j++)
			{
				faceIndices[inidx++] = j;
				faceIndices[inidx++] = numIdx-1;
				if (j == numIdx-2)
				{
					faceIndices[inidx++] = ((LV * 2)* (LH - 1)) + 1;
					break;
				}
				else
				{
					faceIndices[inidx++] = j + 1;
				}
			}
		}
		else
		{
			faceIndices[inidx++] = i;
			faceIndices[inidx++] = (LV * 2) + i;
			if (i == (LV * 2)* numHori)
			{
				faceIndices[inidx++] = ((LV * 2) * numHori) + 1;
			}
			else
			{
				faceIndices[inidx++] = (LV * 2) + i+1;
			}

			faceIndices[inidx++] = i;
			if (i == (LV * 2)* numHori)
			{
				faceIndices[inidx++] = ((LV * 2)* (numHori)) + 1;
				faceIndices[inidx++] = ((LV * 2)* (numHori-1))+1;
				numHori++;
			}
			else
			{
				faceIndices[inidx++] = (LV * 2) + i + 1;
				faceIndices[inidx++] = i + 1;
			}
		}
	}
	cout << "\n";
	//------------------------------------INICIO

	//-------SHADER
	sphere_shader_program_id = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (sphere_shader_program_id == 0)
	{
		cout << "ERROR: Unable to load shader for object" << endl;
		return;
	}
	float *normalData = new float[numVerteData];
	float *UVData = new float[numVerteData];
	for (int i = 0; i < numVertex; i++)
	{
		normalData[i] = 0;
			UVData[i] = 0;
	}
	unsigned short *uvIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices
	unsigned short *normalIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices

	int num = 0;
	for (int i = 0; i < numFaceIndices; i++)
	{
		uvIndices[i] = 0;
		normalIndices[i] = 0;
		//normalIndices[i++] = num;
		//uvIndices[i] = 0;
		//normalIndices[i++] = num;
		//uvIndices[i] = 0;
		//normalIndices[i++] = num;
		//num++;
	}
	bool loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&sphere_shader_program_id, //unsigned int *shaderProgramId
		&sphere_vertex_array_object, //unsigned int *vertexArrayObjectID
		vertexData, //GLfloat *vertices
		numVertex, //int numVertices
		normalData, //GLfloat *normals
		numVertex, //int numNormals
		UVData, //GLfloat *UVcoords
		numIdx, //int numUVCoords,
		faceIndices, //unsigned short *indicesVertices
		numFaceIndices / 3, //int numIndicesVert,
		normalIndices, //unsigned short *indicesNormals
		numFaceIndices / 3, //int numIndicesNormals,
		uvIndices, //unsigned short *indicesUVCoords
		numFaceIndices / 3 //int numIndicesUVCoords	
		);
	if (!loadedToGraphicsCard)
	{
		cout << "ERROR: Unable to save geometry to graphics card" << endl;

		m_numFaces = 0;

		if (sphere_vertex_array_object > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&sphere_vertex_array_object);
			sphere_vertex_array_object = 0;
		}
	}
	//delete[] vertexData;
	//delete faceIndices;
	//delete[] normalIndices;
	//delete[] uvIndices;
	//delete[] UVData;
}

void CAppMy_Sphere_Toroide::CreatePiramid()
{
	int m_numFaces = 6;
	int numVertex = 5;
	int numvertexdata = numVertex * 3;
	int idx = 0;
	float piramideSize = 2.0f;

	float *vertexData = new float[numvertexdata];

	//clear memory
	memset(vertexData, 0, sizeof(float) * numvertexdata);

	vertexData[0] = 0; vertexData[1] = 0; vertexData[2] = 2;//punta 0
	vertexData[3] = -1; vertexData[4] = 1; vertexData[5] = 0;//atras derecha 1
	vertexData[6] = 1; vertexData[7] = 1; vertexData[8] = 0;//atraas izquierda 2 
	vertexData[9] = 1; vertexData[10] = -1; vertexData[11] = 0;//delante derecha 3
	vertexData[12] = -1; vertexData[13] = -1; vertexData[14] = 0;//delante izquierda 4

	int numFaceIndices = m_numFaces * 3;
	unsigned short *faceIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices

	//clear memory
	memset(faceIndices, 0, sizeof(short) * numFaceIndices);

	faceIndices[0] = 0; faceIndices[1] = 1; faceIndices[2] = 2;
	faceIndices[3] = 0; faceIndices[4] = 2; faceIndices[5] = 3;
	faceIndices[6] = 0; faceIndices[7] = 3; faceIndices[8] = 4;
	faceIndices[9] = 0; faceIndices[10] = 4; faceIndices[11] = 1;
	faceIndices[12] = 1; faceIndices[13] = 4; faceIndices[14] = 2;
	faceIndices[15] = 2; faceIndices[16] = 4; faceIndices[17] = 3;

	int numNormals = m_numFaces;
	int numNormalfaces = numNormals * 3;

	float *normalData = new float[numNormalfaces]; // reserve memory for normal data

	// clear memory
	memset(normalData, 0, sizeof(float) * numNormalfaces);

	int numFaceNormalIndices = m_numFaces * 3;
	unsigned short *faceNormalIndices = new unsigned short[numFaceNormalIndices]; // reserve memory for the face UV indices

	// clear memory
	memset(faceNormalIndices, 0, sizeof(unsigned short) * numFaceNormalIndices);

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

	int    numVertexUVs = 5;
	int    numVertexUVData = 5 * 2;
	float *vertexUVData = new float[numVertexUVData];

	// clear memory
	memset(vertexUVData, 0, sizeof(float) * numVertexUVData);

	int             numFaceUVIndices = m_numFaces * 3;
	unsigned short *faceUVIndices = new unsigned short[numFaceUVIndices];

	// clear memory
	memset(faceUVIndices, 0, sizeof(unsigned short) * numFaceUVIndices);


//INICIO PIRAMIDE

	sphere_shader_program_id = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (sphere_shader_program_id == 0)
	{
		cout << "ERROR: Unable to load shader for object" << endl;
		return;
	}
	float *normalData2 = new float[numvertexdata];
	float *UVData = new float[numvertexdata];
	for (int i = 0; i < numVertex; i++)
	{
		normalData[i] = 0;
		UVData[i] = 0;
	}
	unsigned short *uvIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices
	unsigned short *normalIndices = new unsigned short[numFaceIndices]; // reserve memory for the face indices

	int num = 0;
	for (int i = 0; i < numFaceIndices; i++)
	{
		uvIndices[i] = 0;
		normalIndices[i] = 0;
	}
	bool loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&sphere_shader_program_id,   // Shader to use for this object
		&sphere_vertex_array_object, // NEW ID that is going to be created when saving this geometry to graphics memory
		vertexData,               // Vertices
		numVertex,              // Number of vertices (internal code multiplies sizeof(float) * numVertices * 3)
		normalData2,               // Normals
		numNormals,               // Number of normals, (internal code multiplies sizeof(float) * numNormals * 3)
		vertexUVData,             // Texture coordinates
		numVertexUVs,             // Number of tex. coords. (internal code multiplies sizeof(float) * numVertexUVs * 2)
		faceIndices,              // Triangle indices
		numFaceIndices / 3,       // Number of tri. indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
		faceNormalIndices,        // Normal indices
		numFaceNormalIndices / 3, // Number of normal indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
		faceIndices,            // Texture coordinates indices
		numFaceUVIndices / 3      // Number of tex. coord. indices (internal code multiplies sizeof(unsigned short) * num * 3) (numFaces works too)
	);

	if (!loadedToGraphicsCard)
	{
		cout << "ERROR: Unable to save geometry to graphics card" << endl;

		m_numFaces = 0;

		if (sphere_vertex_array_object > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&sphere_vertex_array_object);
			sphere_vertex_array_object = 0;
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
			sphere_texture_id = 0;
			if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &sphere_texture_id, getOpenGLRenderer()))
			{
				cout << "ERROR: Unable load texture for textured cube" << endl;
				sphere_texture_id = 0;
			}
		}
		else
		{
			cout << "ERROR: Unable to find one or more resources: " << endl;
			cout << "  " << MC_GRASS_3_TEXTURE << endl;
		}
	}

	//delete[] vertexData;
	//delete[] faceIndices;
	//delete[] normalData;
	//delete[] faceNormalIndices;
	//delete[] vertexUVData;
	//delete[] faceUVIndices;
}

/* */
void CAppMy_Sphere_Toroide::initialize()
{
	//Código para generar la esfera
	
	CreateSphere();
	//CreatePiramid();
}

/* */
void CAppMy_Sphere_Toroide::run()
{
	
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_ESFERA))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppMy_Sphere_Toroide::update(double deltaTime)
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
void CAppMy_Sphere_Toroide::render()
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
		if (m_numFaces > 0 && sphere_vertex_array_object > 0 && sphere_shader_program_id > 0)
		{
			// White 
			// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
			float color[3] = { 1.0f, 1.0f, 1.0f };

			// Position object in (0, 0, 0)
			CVector3 pos = {0,0,1};
			//CVector3 pos = CVector3::ZeroVector();

			// Angle in RADIANS! (multiply * PI / 180.0f to convert to radians)
			float rotationAroundY = 125.0f * PI_OVER_180;

			// Create a simple model matrix, with given position and rotation of zero degrees around Y axis
			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(rotationAroundY, pos);
			modelMatrix = {
			cos(time),0,sin(time),0,
			0,1,0,0,
			sin(time),0,cos(time),0,
			0,0,0,1
			};
			modelMatrix = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
			};
			time += 0.001;
			//modelMatrix *= MathHelper::RotAroundY(time);
			// Render the cube
			getOpenGLRenderer()->renderObject(
				&sphere_shader_program_id,                      // Shader to use to render the object
				&sphere_vertex_array_object,                    // ID for the geometry
				&sphere_texture_id,                            // ID for the texture
				m_numFaces,                                  // Number of triangles that the object has
				color,                                       // Color (tint)
				&modelMatrix,                                // Object transformation
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES, //
				false                                        //
			);
		}
		// =================================
		//
		// =================================
	}
	
}

/* */
void CAppMy_Sphere_Toroide::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppMy_Sphere_Toroide::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}