#pragma once

#ifndef CAPPFBX_LOADER_H
#define CAPPFBX_LOADER_H

#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"
#include "CVector3.h"

/* ******************************************************************************************************************************************
* Está clase es para poder cargar un archivo 3D (.FBX)
* ******************************************************************************************************************************************/

class CAppFBX_Loader : public CApp
{
	protected:
		// Method to initialize the menu
		bool initializeMenu();
	
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

		// Load/unload 3D model
		bool load3DModel(const char * const file_name);
		void unloadCurrent3DModel();

		// Move camera away/closer
		void moveCamera(float direction);

	public:
		CAppFBX_Loader();
		CAppFBX_Loader(int window_width, int window_height);
		~CAppFBX_Loader(); 


		// Method to initialize any objects for this class
		void initialize();

		// Method to update any objecs based on time elapsed since last frame
		void update(double delta_Time);

		// Method to run the app
		void run();

		// Method to render the app
		void render();

		// Executes the action corresponding to the selected menu item
		void executeMenuAction();

		// Called when mouse moves within the window
		void onMouseMove(float delta_X, float delta_Y);

		// This derived class only uses F2/F3
		void onF2(int mods);
		void onF3(int mods);
};


//IMPORTANTE
#endif // !CAPPFBX_LOADER_H