#pragma once

#ifndef CAPPHEXGRID_H
#define CAPPHEXGRID_H

#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"
#include "CVector3.h"


class CAppHexGrid : public CApp
{
	protected:
		bool initializeMenu();// Method to initialize the menu
	
	private:
		C3DModel *m_p3DModel;// Pointer to an object of type C3DModel
	
		double m_currentDeltaTime;// Current delta time (time of the current frame - time of the last frame)
	
		double m_objectRotation;// Current object rotation, expressed in degrees
	
		CVector3 m_objectPosition;// Current object position
		
		double m_rotationSpeed;//  Object rotation speed (degrees per second)
	
		// Load/unload 3D model
		bool load3DModel(const char * const file_name);
		void unloadCurrent3DModel();
		
		void moveCamera(float direction);// Move camera away/closer
	
	public:
		CAppHexGrid(int window_width, int window_height);
		CAppHexGrid();
		~CAppHexGrid();
	
	
		void initialize();// Method to initialize any objects for this class
		
		void update(double delta_Time);// Method to update any objecs based on time elapsed since last frame
	
		void run();// Method to run the app
		
		void render();// Method to render the app
	
		void executeMenuAction();// Executes the action corresponding to the selected menu item
	
		void onMouseMove(float delta_X, float delta_Y);// Called when mouse moves within the window
	
		// This derived class only uses F2/F3
		void onF2(int mods);
		void onF3(int mods);
};


//IMPORTANTE
#endif // !CAPPHEXGRID_H