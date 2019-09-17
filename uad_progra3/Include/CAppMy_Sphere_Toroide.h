#pragma once

#ifndef CAPPMY_SPHERE_TOROIDE_H
#define CAPPMY_SPHERE_TOROIDE_H

#include "Globals.h"
#include "CApp.h"


// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CAppMy_Sphere_Toroide : public CApp
{
	private:
		// ---------------------------------------------------
		// Private member variables specific to CAppMy_Sphere_Toroide only
		// ---------------------------------------------------
		
		unsigned int sphere_shader_program_id;
		unsigned int sphere_vertex_array_object;
		unsigned int sphere_texture_id=0;
		
		int m_numFaces;
	
		// ---------------------------------------------------
	
	protected:
		// Method to initialize the menu 
		// (not needed in CAppMy_Sphere_Toroide, so return false)

		bool initializeMenu() { return false; }
	
	public:
		// Constructors and destructor
		CAppMy_Sphere_Toroide();
		CAppMy_Sphere_Toroide(int window_width, int window_height);
		~CAppMy_Sphere_Toroide();

	
		// --------------------------------------------------------------------------------------------------------------------------
		// Inherited methods from CApp
		// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
		// --------------------------------------------------------------------------------------------------------------------------
	
		void CreateSphere();
		void CreatePiramid();
		// Method to initialize any objects for this class
		void initialize();
		
		float time = 0;

		// Method to update any objects based on time elapsed since last frame
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
		// Public methods specific to CAppMy_Sphere_Toroide only
		// -----------------------------------------
	
	private:
	
		// Private methods specific to CAppMy_Sphere_Toroide only
		// ------------------------------------------
	
};

#endif // !CAPPMY_SPHERE_TOROIDE_H