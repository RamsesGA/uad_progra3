#pragma once

#ifndef CAPPPARCIAL2_H
#define CAPPPARCIAL2_H

#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"
#include "CVector3.h"

// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
class CAppParcial2 : public CApp
{
private:
	// Pointer to an object of type C3DModel
	C3DModel *m_p3DModel;

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	// Current object rotation, expressed in degrees
	double m_objectRotation; 

	// Current object position
	CVector3 m_objectPosition;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

	// Shader program for the current model
	unsigned int m_currentModelShaderId;

	// Texture object ID for the current model
	unsigned int m_currentModelTextureObject;

	// Texture object ID for minecraft cube
	unsigned int m_mcCubeTextureID;

protected:
	// Method to initialize the menu
	bool initializeMenu();
	// Method to initialize a MC cube and its texture
	bool initializeMCCube();

public:
	// Constructors and destructor
	CAppParcial2();
	CAppParcial2(int window_width, int window_height);
	~CAppParcial2();

	// Inherited methods from CApp
	// ---------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// Methods specific to CAppParcial2 only
	// -------------------------------------

	// This derived class only uses F2/F3
	void onF2(int mods);
	void onF3(int mods);
	
private:

	// Load/unload 3D model
	bool load3DModel(const char * const filename);
	void unloadCurrent3DModel();

	// Move camera away/closer
	void moveCamera(float direction);

	//
	bool loadTexture(const char *filename, unsigned int *newTextureID);
};

#endif // !CAPPPARCIAL2_H