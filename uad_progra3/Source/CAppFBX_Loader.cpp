#include "../stdafx.h"
#include "windows.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "..\Include\CAppFBX_Loader.h"
#include "../Include/CTextureLoader.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/C3DModel.h"
#include "../Include/Globals.h"



bool CAppFBX_Loader::initializeMenu()
{
	cout << "CAPP \nF B X" << endl;

	wstring wresourceFilenameTexture;
	string resourceFilenameTexture;

	if (!CWideStringHelper::GetResourceFullPath(MENU_TEXTURE_FILE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: no se puede encontrar uno o mas recursos: " << endl;
		cout << " --> " << MENU_TEXTURE_FILE << endl;
		return false;
	}

	if (getMenu() != nullptr)
	{
		CGameMenu *const menu = getMenu();

		bool generated_Menu_Item_Geometry = false;

		int texture_Uniform_Location = -1;
		int color_Uniform_Location = -1;

		unsigned int menu_Shader_Program_Id = 0;
		unsigned int texture_Object_Id = 0;
		unsigned int vao_Menu_Item_Id = 0;
		
		float current_X = 0.65f;
		float current_Y = 0.8f;
		float delta_Y = 0.25f;
		float menu_Item_Height = 0.2f;
		float UV[8];

		menu_Shader_Program_Id = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_MENU);

		// Create a shader program to use for the menu
		if (menu_Shader_Program_Id == 0)
		{
			cout << "ERROR: Unable to load menu shader program" << endl;
			return false;
		}
		// Set the generated shader program in the menu object
		menu->setShaderProgramId(menu_Shader_Program_Id);

		// Read texture file and generate an OpenGL texture object
		if (CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &texture_Object_Id, getOpenGLRenderer()))
		{
			// Set the generated texture object in the menu object
			menu->setTextureObjectId(texture_Object_Id);
		}
		else
		{
			return false;
		}

		vector<string> menuOptions;
		menuOptions.push_back("---Load - 3D - Model---\n");
		menuOptions.push_back("---Options---\n");
		menuOptions.push_back("---Exit---\n");

		UV[0] = 0.01f;	// top left U
		UV[1] = 1.0f;   // top left V
		UV[2] = 0.99f;  // top right U
		UV[3] = 1.0f;   // top right V
		UV[4] = 0.01f;  // bottom left U
		UV[5] = 0.75f;  // bottom left V
		UV[6] = 0.99f;  // bottom right U
		UV[7] = 0.75f;  // bottom right V

		for (size_t i = 0; i < menuOptions.size(); i++)
		{
			// Allocate graphics memory for menu item
			generated_Menu_Item_Geometry = getOpenGLRenderer()->allocateGraphicsMemoryForMenuItem(current_X, current_Y, menu_Item_Height, UV, &menu_Shader_Program_Id, &vao_Menu_Item_Id);

			// If operation failed
			if (!generated_Menu_Item_Geometry)
			{
				menu->cleanupGraphicsObjects(getOpenGLRenderer());
				return false;
			}

			menu->addMenuItem(menuOptions[i].c_str(), current_X, current_Y, vao_Menu_Item_Id);
			current_Y -= delta_Y;

			UV[1] -= 0.25f;
			UV[3] -= 0.25f;
			UV[5] -= 0.25f;
			UV[7] -= 0.25f;

			// Custom hack, texture used is the same for the exam, but the "exit" option is the 4th, not 3rd, so increment twice
			if (i == 1)
			{
				UV[1] -= 0.25f;
				UV[3] -= 0.25f;
				UV[5] -= 0.25f;
				UV[7] -= 0.25f;
			}
		}
		return true;
	}
	return false;
}


bool CAppFBX_Loader::load3DModel(const char * const file_name)
{
	unsigned int model_Vertex_Array_Object = 0;

	// Unload any current 3D model
	unloadCurrent3DModel();

	// Create new 3D object
	m_p3DModel = C3DModel::load(file_name, getOpenGLRenderer());

	if (m_p3DModel == nullptr)
	{
		cout << "ERROR: no se puede leer el modelo del archivo" << endl;
		return false;
	}
	else if (!m_p3DModel->isGeometryLoaded())
	{
		cout << "ERROR: no se puede leer la geometria del modelo." << endl;
		unloadCurrent3DModel();
		return false;
	}
	else if (m_p3DModel->getGraphicsMemoryObjectId() == 0)
	{
		cout << "ERROR: no se puede guardar la geometria en la tarjeta grafica." << endl;
		unloadCurrent3DModel();
		return false;
	}
	return (m_p3DModel->getGraphicsMemoryObjectId() > 0);
}


void CAppFBX_Loader::unloadCurrent3DModel()
{
	if (m_p3DModel != nullptr)
	{
		// Free up graphics memory
		unsigned int vao_ID = m_p3DModel->getGraphicsMemoryObjectId(); 

		getOpenGLRenderer()->freeGraphicsMemoryForObject(&vao_ID);

		// Free up texture object memory
		if (m_p3DModel->getTextureObjectId() > 0)
		{
			unsigned int tex_ID = m_p3DModel->getTextureObjectId();
			getOpenGLRenderer()->deleteTexture(&tex_ID);
		}

		// Delete 3D object
		delete m_p3DModel;
		m_p3DModel = nullptr;
	}
}


void CAppFBX_Loader::moveCamera(float direction)
{
	if (getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->simpleCameraZoom(direction);
	}
}


CAppFBX_Loader::CAppFBX_Loader() : CAppFBX_Loader(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
	cout << "Iniciando el constructor sin parametros: CAppFBX_Loader()" << endl;
}


CAppFBX_Loader::CAppFBX_Loader(int window_width, int window_height) : CApp(window_width, window_height), m_p3DModel(nullptr), m_currentDeltaTime{ 0.0 }, m_objectRotation{ 0.0 }, m_objectPosition{ 0.0f, 0.0f, 0.0f }, m_rotationSpeed{ DEFAULT_ROTATION_SPEED }
{
	cout << "Iniciando el constructor con parametros de tamaño definido: CAppFBX_Loader()" << endl;
}


CAppFBX_Loader::~CAppFBX_Loader()
{
	cout << "Destructor: ~CAppFBX_Loader()" << endl;
	unloadCurrent3DModel();
}


void CAppFBX_Loader::initialize(){}


void CAppFBX_Loader::update(double delta_Time)
{
	double degrees_To_Rotate = 0.0f;

	if (delta_Time <= 0.0f)
	{
		return;
	}

	// Save current delta time
	m_currentDeltaTime = delta_Time;

	// Calculate degrees to rotate
	// degrees = rotation speed * delta_Time

	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degrees_To_Rotate = m_rotationSpeed * (delta_Time / 1000.0);

	// accumulate rotation degrees
	m_objectRotation += degrees_To_Rotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0f)
	{
		m_objectRotation -= 360.0f;
	}
	if (m_objectRotation < 0.0f)
	{
		m_objectRotation = 0.0f;
	}
}


void CAppFBX_Loader::run()
{
	// Check if CGameWindow object and third-party Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_FBX_LOADER_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.15f, 0.75f, 0.75f);

			// Create our menu (add all menu items)
			if (!initializeMenu())
			{
				return;
			}

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}


void CAppFBX_Loader::render()
{
	CGameMenu *menu = getMenu();
	CVector3 obj_Pos2;

	obj_Pos2.setValues(m_objectPosition.getX() + 2.5f, m_objectPosition.getY(), m_objectPosition.getZ());

	// If menu is active, render menu
	if (menu != nullptr && menu->isInitialized() && menu->isActive())
	{
		menu->render(getOpenGLRenderer());
	}
	else // Otherwise, render active object if loaded (or test cube if no object is loaded)
	{
		// White 
		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 1.0f, 1.0f, 1.0f };

		if (m_p3DModel != nullptr && m_p3DModel->isGeometryLoaded() && m_p3DModel->getGraphicsMemoryObjectId() > 0)
		{
			// convert total degrees rotated to radians
			double total_Degrees_Rotated_Radians = m_objectRotation * 3.1459 / 180.0;

			// Get a matrix that has both the object rotation and translation
			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)total_Degrees_Rotated_Radians, m_objectPosition);

			unsigned int modelShader = m_p3DModel->getShaderProgramId();
			unsigned int modelVAO = m_p3DModel->getGraphicsMemoryObjectId();
			unsigned int modelTexture = m_p3DModel->getTextureObjectId();

			getOpenGLRenderer()->renderObject(&modelShader, &modelVAO, &modelTexture, m_p3DModel->getNumFaces(), color, &modelMatrix, COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES, false);
		}
	}
}


void CAppFBX_Loader::executeMenuAction()
{
	if (getMenu() != nullptr)
	{
		int option = getMenu()->getSelectedMenuItemNumber();

		switch (option)
		{
			case 1:
				onF2(0);
				break;
			case 2:
				// Not implemented
				cout << "<MENU OPTION NOT IMPLEMENTED>" << endl;
				break;
			case 3:
				if (getGameWindow() != nullptr)
				{
					getGameWindow()->requestWindowClose();
				}
				break;
		}
	}
}


void CAppFBX_Loader::onMouseMove(float delta_X, float delta_Y)
{
	if (delta_X < 100.0f && delta_Y < 100.0f)
	{
		float moveX = -delta_X * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -delta_Y * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];

		m_objectPosition.getValues(currPos);
		currPos[0] += moveX;
		currPos[2] += moveZ;
		m_objectPosition.setValues(currPos);
	}
}

void CAppFBX_Loader::onF2(int mods)
{
	setMenuActive(true);

	wstring wide_String_Buffer = L"";

	wide_String_Buffer.resize(MAX_PATH);

	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L" Obj Files\0*.obj\0 Stl Files\0*.stl\0 3DS Files\0*.3ds\0 All files\0*.*\0";
	ofn.lpstrFile = &wide_String_Buffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_String_Buffer[0], (int)wide_String_Buffer.size(), NULL, 0, NULL, NULL);
		string multibyteString(size_needed, 0);

		WideCharToMultiByte(CP_UTF8, 0, &wide_String_Buffer[0], (int)wide_String_Buffer.size(), &multibyteString[0], size_needed, NULL, NULL);
		
		cout << "Filename to load: --> " << multibyteString.c_str() << endl;

		if (!load3DModel(multibyteString.c_str()))
		{
			cout << "\nNo se puede cargar el modelo 3D " << endl;
		}
		else
		{
			setMenuActive(false);
		}
	}
}

void CAppFBX_Loader::onF3(int mods)
{
	// Check BITWISE AND to detect shift/alt/ctrl
	if (mods & KEY_MOD_SHIFT)
	{
		moveCamera(-1.0f);
	}
	else
	{
		moveCamera(1.0f);
	}
}
