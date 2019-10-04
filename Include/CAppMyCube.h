#pragma once

#ifndef CAPPMYCUBE_H
#define CAPPMYCUBE_H

#include "Globals.h"
#include "CApp.h"

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CAppMyCube : public CApp
{
	private:
	
		// ---------------------------------------------------
		// Private member variables specific to CAppEmpty only
		// ---------------------------------------------------
		//
	
		unsigned int m_cubeShaderProgramID;
		unsigned int m_cubeVertexArrayObject;
		unsigned int m_cubeTextureID;
		int m_numFaces;
	
		// ---------------------------------------------------
	
	protected:
	
		// Method to initialize the menu 
		// (not needed in CAppEmpty, so return false)
		bool initializeMenu() { return false; }
	
	public:
		// Constructors and destructor
		CAppMyCube();
		CAppMyCube(int window_width, int window_height);
		~CAppMyCube();
	
		// --------------------------------------------------------------------------------------------------------------------------
		// Inherited methods from CApp
		// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
		// --------------------------------------------------------------------------------------------------------------------------
	
		//Metodos heredados de CApp
		//------------------------------------------------------------------------------------------------------------------------
		// Method to initialize any objects for this class
		void initialize();
	
		// Method to update any objecs based on time elapsed since last frame
		void update(double deltaTime);
	
		// Method to run the app
		void run();
	
		// Method to render the app
		void render();
	
		// ------------------------------------------------------------------------------------------------------------------------
		
		// Other inherited methods from CApp. These are optional to override.
		// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
		// -----------------------------------------------------------------------------------------------------------------------
	
		// Executes the action corresponding to the selected menu item
		void executeMenuAction();
	
		// Called when mouse moves within the window
		void onMouseMove(float deltaX, float deltaY);
	
		// -----------------------------------------
		// Public methods specific to CAppEmpty only
		// -----------------------------------------
		//
		// -----------------------------------------
	
	private:
	
		// ------------------------------------------
		// Private methods specific to CAppEmpty only
		// ------------------------------------------
		//
		// ------------------------------------------
	
};

#endif // !CAPPMYCUBE_H

