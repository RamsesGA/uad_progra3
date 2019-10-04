#include "../stdafx.h"

#include "windows.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppCubeTest.h"
#include "../Include/C3DModel.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"

/* */
CAppCubeTest::CAppCubeTest() :
	CAppCubeTest(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT) // C++11 ability to call one constructor from another
{
	cout << "Constructor: CAppCubeTest()" << endl;
}

/* */
CAppCubeTest::CAppCubeTest(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, 0.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED }
{
	cout << "Constructor: CAppCubeTest(int window_width, int window_height)" << endl;
}

/* */
CAppCubeTest::~CAppCubeTest()
{
	cout << "Destructor: ~CAppCubeTest()" << endl;

	if (m_texturedCubeTextureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_texturedCubeTextureID);
	}
}

/* */
void CAppCubeTest::run()
{
	// Check if CGameWindow object and third-party Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_CUBES_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.15f, 0.75f, 0.75f);

			// Initialize a test cube
			getOpenGLRenderer()->initializeColorCube();

			// Initialize cubes
			if (!initializeTexturedCube())
			{
				return;
			}

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
bool CAppCubeTest::initializeTexturedCube()
{
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_GRASS_DIRT_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_GRASS_DIRT_TEXTURE << endl;
		return false;
	}

	// Initialize the texture
	m_texturedCubeTextureID = 0;
	if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &m_texturedCubeTextureID, getOpenGLRenderer()))
	{
		cout << "ERROR: Unable load texture for textured cube" << endl;
		return false;
	}

	// Initialize a textured cube
	getOpenGLRenderer()->initializeTexturedCube();

	return true;
}

/* */
void CAppCubeTest::update(double deltaTime)
{
	double degreesToRotate = 0.0;

	if (deltaTime <= 0.0f)
	{
		return;
	}

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

/* */
void CAppCubeTest::render()
{
	CVector3 objPos2;
	objPos2.setValues(m_objectPosition.getX() + 2.5f, m_objectPosition.getY(), m_objectPosition.getZ());

	// convert total degrees rotated to radians;
	double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

	float cosine = cosf((float)totalDegreesRotatedRadians);
	float sine = sinf((float)totalDegreesRotatedRadians);

	MathHelper::Matrix4 sm = MathHelper::ScaleMatrix(
		0.5f, 0.5f, 0.5f
	);

	MathHelper::Matrix4 tm = MathHelper::TranslationMatrix(
		m_objectPosition.X, m_objectPosition.Y, m_objectPosition.Z
	);

	MathHelper::Matrix4 rx = MathHelper::RotAroundX((float)totalDegreesRotatedRadians);
	MathHelper::Matrix4 ry = MathHelper::RotAroundY((float)totalDegreesRotatedRadians);
	MathHelper::Matrix4 finalRotation = MathHelper::Multiply(rx, ry);
	MathHelper::Matrix4 rotationAndScale = MathHelper::Multiply(finalRotation, sm);
	MathHelper::Matrix4 modelMatrix = MathHelper::Multiply(rotationAndScale, tm);

	CVector3 pos2 = m_objectPosition;
	pos2 += CVector3(3.0f, 0.0f, 0.0f);
	MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, pos2);

	// Look at 0,0,0 from 0,8,8
	CVector3 camEyePos = CVector3::ZeroVector();
	CVector3 camLookAt = CVector3::ZeroVector();
	CVector3 camUpVec  = CVector3::ZeroVector();
	camEyePos.Y = 8.0f;
	camEyePos.Z = 8.0f;
	camUpVec.Y = 1.0f;

	// Construct a view matrix
	MathHelper::Matrix4 viewM = MathHelper::ViewMatrix(
		camEyePos, camLookAt, camUpVec
	);

	float aspectRatio = (float)getOpenGLRenderer()->getFramebufferWidth() / (float)getOpenGLRenderer()->getFramebufferHeight();

	// Construct a projection matrix
	MathHelper::Matrix4 projM = MathHelper::PerspectiveProjectionMatrix(
		75.0f, aspectRatio, 0.1f, 100.0f
	);

	// No model loaded, show test cubes
	//getOpenGLRenderer()->renderColorCube(&modelMatrix);
	getOpenGLRenderer()->renderColorCube(&modelMatrix, &viewM, &projM);
	
	//getOpenGLRenderer()->renderTexturedCube(m_texturedCubeTextureID, &modelMatrix2);
	getOpenGLRenderer()->renderTexturedCube(m_texturedCubeTextureID, &modelMatrix2, &viewM, &projM);
}

/* */
void CAppCubeTest::onMouseMove(float deltaX, float deltaY)
{
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		m_objectPosition.getValues(currPos);
		currPos[0] += moveX;
		currPos[2] += moveZ;
		m_objectPosition.setValues(currPos);
	}
}

