#pragma once

#ifndef CAPPGEOMETRICFIGURES_H
#define CAPPGEOMETRICFIGURES_H

#include "Globals.h"
#include "CApp.h"

/* ******************************************************************************************************************************************
* This class shows an example of how to manually allocate GRAPHICS memory for a 3D object.
* The object is rendered twice, one with colors only, and one with a texture.
* Each of these ways of rendering an object needs a different shader.
* Displaying more than one of those objects in screen can be achieved by making two render calls, using different positions, but
* using the same graphics memory reference of the cube.
* ******************************************************************************************************************************************/

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CAppGeometricFigures : public CApp
{
private:

	bool m_initialized;

	// Variables used for controlling the rotation and object position
	// ---------------------------------------------------------------

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

	// Variables used for storing OpenGL identifiers
	// ---------------------------------------------------------------

	// OpenGL Shader Program for the color-only object
	unsigned int m_colorModelShaderId;

	// OpenGL Shader Program for the textured object
	unsigned int m_texturedModelShaderId;

	// An OpenGL Vertex Array Object is a reference to graphics memory that tells OpenGL where to look for the geometry of the object
	// It needs to be initialized using the allocateGraphicsMemory() method from the renderer.
	unsigned int m_pyramidVertexArrayObject;
	
	// Identifier for the OpenGL Texture Object 
	unsigned int m_textureID;

	// Other variables
	// ---------------------------------------------------------------

	int m_numFacesPyramid;
	int m_renderPolygonMode;

protected:

	// Method to initialize the menu 
	// (not needed here, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	CAppGeometricFigures();
	CAppGeometricFigures(int window_width, int window_height);
	~CAppGeometricFigures();

	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// -----------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	//void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// Override on F2/F3
	void onF2(int mods);
	void onF3(int mods);

private:

	int m_jetas_hexa;
	void createPyramidGeometry();
	CVector3 createHexaGemoetry(CVector3 _center, int _indice, float _cell_size, bool _p);
	void structureHexa();

};

#endif // !CAPPGEOMETRICFIGURES_H