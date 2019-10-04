#pragma once

#ifndef CAPPEMPTY_H
#define CAPPEMPTY_H

#include "Globals.h"
#include "CApp.h"

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CAppEmpty : public CApp
{
private:

	// ---------------------------------------------------
	// Private member variables specific to CAppEmpty only
	// ---------------------------------------------------
	//
	// ---------------------------------------------------

protected:

	// Method to initialize the menu 
	// (not needed in CAppEmpty, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	CAppEmpty();
	CAppEmpty(int window_width, int window_height);
	~CAppEmpty();

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

#endif // !CAPPEMPTY_H