#pragma once

#ifndef CAPPCUBETEST_H
#define CAPPCUBETEST_H

#include "Globals.h"
#include "CApp.h"
#include "CVector3.h"

/* ******************************************************************************************************************************************
 * This class shows an example of how to display two cubes on screen, one of them has a texture, and the other one has colours only
 * Each of these cubes is rendered using a diferent Shader. 
 * The OpenGL renderer has a helper method to create the geometry for the cube, but it can be done manually (see CAppGeometricShapes).
 * Displaying more than one of those objects in screen can be achieved by making two render calls, using different positions, but 
 * using the same graphics memory reference of the cube.
 * ******************************************************************************************************************************************/

// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
class CAppCubeTest : public CApp
{
private:

	// Variables used for controlling the rotation and object position
	// ---------------------------------------------------------------

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Object rotation speed (degrees per second)
	double m_rotationSpeed;

	// Current object position
	CVector3 m_objectPosition;

	// -----------------------------------------------------------------
	
	// Texture object ID for textured cube
	unsigned int m_texturedCubeTextureID;

protected:

	bool initializeMenu() { return false; }

	// Method to initialize a textured cube and its texture
	bool initializeTexturedCube();

public:
	// Constructors and destructor
	CAppCubeTest();
	CAppCubeTest(int window_width, int window_height);
	~CAppCubeTest();

	// Inherited methods from CApp
	// ---------------------------

	// Method to initialize any objects for this class
	void initialize() {}

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

private:

};

#endif // !CAPPCUBETEST_H