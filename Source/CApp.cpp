#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "../Include/CApp.h"
#include "../Include/LoadTGA.h"

/* */
CApp::CApp(): CApp(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CApp::CApp(int window_width, int window_height)
{
	cout << "Constructor: CApp(int window_width, int window_height)" << endl; 
	
	// Create OpenGLRenderer Object
	m_OpenGLRenderer = new COpenGLRenderer();

	// Create CGameWindow object
	m_Window = new CGameWindow(m_OpenGLRenderer, window_width, window_height);

	// Create CGameMenu Object
	// Menu will be initialized later (menu items added)
	m_Menu = new CGameMenu();
	
	// Create default light
	m_DefaultLight = new CLight(DEFAULT_LIGHT_COLOR_RED, DEFAULT_LIGHT_COLOR_GREEN, DEFAULT_LIGHT_COLOR_BLUE, DEFAULT_AMBIENT_LIGHT_INTENSITY);
}

/* */
CApp::~CApp()
{
	cout << "Destructor: ~CApp()" << endl;

	// Free CGameWindow object resources
	if (m_Window != NULL)
	{
		cout << "Delete m_Window" << endl;
		delete m_Window;
	}

	m_Window = NULL;

	// Free CGameMenu object resources
	if (m_Menu != NULL)
	{
		// Cleanup call deleteVertexArrayObject
		m_Menu->cleanupGraphicsObjects(m_OpenGLRenderer);

		cout << "Delete m_Menu" << endl;
		delete m_Menu;
	}

	m_Menu = NULL;

	// Free COpenGLRenderer object resources
	if (m_OpenGLRenderer != NULL)
	{
		cout << "Delete m_OpenGLRenderer" << endl;
		delete m_OpenGLRenderer;
	}

	m_OpenGLRenderer = NULL;
	
	if (m_DefaultLight != nullptr)
	{
		delete m_DefaultLight;
	}

	m_DefaultLight = nullptr;
}

/* */
bool CApp::isWindowInitialized() const
{
	if (m_Window != NULL)
	{
		return m_Window->isLibraryInitialized();
	}

	return false;
}

/* */
bool CApp::canRun() const
{
	return isWindowInitialized();
}

/* */
bool CApp::isMenuActive() const
{
	if (m_Menu != NULL)
	{
		return m_Menu->isActive();
	}

	return false;
}

/* */
void CApp::setMenuActive(bool active)
{
	if (m_Menu != NULL)
	{
		m_Menu->setActive(active);

		if (active)
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.88f, 0.88f, 0.88f);
			}

			cout << "Menu is active" << endl;
			cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
		}
		else
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.15f, 0.75f, 0.75f);
			}

			cout << "Menu is NOT active" << endl;
		}
	}
}

/* */
void CApp::executeMenuAction()
{
	if (m_Menu != NULL)
	{
		cout << "Execute Menu action" << endl;
	}
}

/* */
void CApp::selectNextMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(true);
		cout << "Select next menu option" << endl;
		cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}

/* */
void CApp::selectPrevMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(false);
		cout << "Select prev menu option" << endl;
		cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}
