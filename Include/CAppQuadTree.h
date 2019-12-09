#pragma once

#ifndef CAPPQUEADTREE_H
#define CAPPQUEADTREE_H

#include "Globals.h"
#include "CApp.h"
#include <conio.h>

#include "../Include/Hexa_World.h"
#include "../Include/CQuadTree.h"
#include "..\Include\CCamera.h"

class CAppQuadTree : public CApp
{
	private:
	
	protected:
	
		// Method to initialize the menu (not needed in CAppQuadTree, so return false)
		bool initializeMenu() { return false; }
	
	public:
		
		//Miembros
		HANDLE threadGrid;
		HANDLE eventGridFinished;
		DWORD threadEndId;
	
		CCamera *m_camera = nullptr;

		unsigned int m_vertexArrayObjectHexaGrid;
		unsigned int m_ColorModelShaderID;
		unsigned int m_texturedModelShaderID;


		int m_cols;
		int m_rows;

		float m_minX;
		float m_maxX;
		float m_minZ;
		float m_maxZ;
	
		Hexa_Grid *m_objHexGrid;
		CQuadTree m_objQuadTree;
		AABB_2D m_boundering;

		// Constructors
		CAppQuadTree();
		CAppQuadTree(int window_width, int window_height);
	
		//Destructor
		~CAppQuadTree();
	
		// Inherited methods from CApp, Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	
		// Method to initialize any objects for this class
		void 
		initialize();

		void
			render();
	
		//Función para poder mandar a llamar la función para crear el Hexa Grid
		bool 
		StartThreadGrid();

		//Acceso a las teclas W,S,A,D, ARRIBA, ABAJO, DERECHA, IZQUIERDA
		void
		onArrowUp(int _key);
							  
		void 				  
		onArrowDown(int _key) ;
							  
		void 				  
		onArrowLeft(int _key) ;
							  
		void 				  
		onArrowRight(int _key);

		// Method to update any objecs based on time elapsed since last frame
		void
		update(double deltaTime);
	
		// Method to run the app
		void 
		run();
	
		// Other inherited methods from CApp. These are optional to override.
		// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	
		// Executes the action corresponding to the selected menu item
		void 
		executeMenuAction();
	
		// Called when mouse moves within the window
		void
		onMouseMove(float deltaX, float deltaY);

		void moveCamera(float _direction);
		void onF3(int mods);
	
};

#endif // !CAPPQUEADTREE_H