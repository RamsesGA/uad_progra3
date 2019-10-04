#pragma once

#ifndef CAPP_H
#define CAPP_H

#include "CGameWindow.h"
#include "CGameMenu.h"
#include "COpenGLRenderer.h"
#include "CLight.h"

#define KEY_MOD_SHIFT     0x0001
#define KEY_MOD_CONTROL   0x0002
#define KEY_MOD_ALT       0x0004
#define KEY_MOD_UNUSED    0x0008

// ================================================================
// One of the MAIN files for this project
// BASE CLASS FOR ALL 'APPLICATIONS'
// The CApp* classes derive from this class and are used in main( )
// ================================================================
class CApp
{
	private:
		CGameWindow     *m_Window;         // Pointer to our CGameWindow object
		CGameMenu       *m_Menu;           // Pointer to our CMenu object
		COpenGLRenderer *m_OpenGLRenderer; // Pointer to our OpenGL renderer object
		CLight          *m_DefaultLight;   // Default light

		bool m_Paused;                     // Is the app paused?  (i,e: window minimized)
		bool isWindowInitialized() const;  // Is the CGameWindow object initialized ?
		
	protected:
		CGameWindow     * const getGameWindow()     const { return m_Window; }
		CGameMenu       * const getMenu()           const { return m_Menu; }
		COpenGLRenderer * const getOpenGLRenderer() const { return m_OpenGLRenderer; }

	protected:
		virtual bool initializeMenu() = 0;
	
	public:
		// Constructors and VIRTUAL destructor
		CApp();
		CApp(int window_width, int window_height);
		virtual ~CApp();
	
		bool canRun() const;                                  // Can the app run?
		bool isMenuActive() const;                            // Is the menu active?
		void setMenuActive(bool active);                      // Activate/deactivate the menu
		bool isAppPaused() const { return m_Paused; }         // Is the app paused?
		void setAppPaused(bool paused) { m_Paused = paused; } // Pause/Un-pause the app
		void selectNextMenuItem();                            // Select next menu item
		void selectPrevMenuItem();                            // Select previous menu item
		virtual void executeMenuAction();                     // Execute the action corresponding to the selected menu item
	
		// Virtual methods                                    // F1 key always toggles the menu on/off, F12 key always closes the app
		// =============================                      // The functionality for the rest of the keys (F2..F11) is specific to the derived class
		virtual void onF2(int mods)  {}                       // F2
		virtual void onF3(int mods)  {}                       // F3
		virtual void onF4(int mods)  {}                       // F4
		virtual void onF5(int mods)  {}                       // F5
		virtual void onF6(int mods)  {}                       // F6
		virtual void onF7(int mods)  {}                       // F7
		virtual void onF8(int mods)  {}                       // F8
		virtual void onF9(int mods)  {}                       // F9
		virtual void onF10(int mods) {}                       // F10
		virtual void onF11(int mods) {}                       // F11
	
		virtual void onArrowUp(int mods) {}
		virtual void onArrowDown(int mods) {}
		virtual void onArrowLeft(int mods) {}
		virtual void onArrowRight(int mods) {}
	
		virtual void onMouseMove(float deltaX, float deltaY) {}
	
		// Methods to run/render the app and initialize the menu
		// *Note: They are PURE VIRTUAL METHODS, so we cannot create an object of this base class (compiler won't allow it),
		//        instead we need to create an object of one of the derived classes, i.e: CAppParcial2
		//        and the derived class MUST override these methods
		virtual void initialize() = 0;
		virtual void update(double delta_time) = 0;
		virtual void run()    = 0;
		virtual void render() = 0;

	
};

#endif // !CAPP_H