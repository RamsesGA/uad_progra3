#include "../stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "../Include/Hexa_World.h"
#include "../Include/Hexa_Grid.h"


//Cositas extra
void Hexa_World::onF3(int mods)
{
	//if (m_render_Polygon_Mode == 0)
	//{
	//	getOpenGLRenderer()->setFillPolygonMode();
	//	m_render_Polygon_Mode = 1;
	//}
	//else
	//{
	//	getOpenGLRenderer()->setWireframePolygonMode();
	//	m_render_Polygon_Mode = 0;
	//}
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

void Hexa_World::moveCamera(float _direction)
{
	if (getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->simpleCameraZoom(_direction);
	}
}

Hexa_World::Hexa_World(int window_width, int window_height) : CApp(window_width, window_height), m_current_Delta_Time{ 0.0 }, m_object_Rotation{ 0.0 }, m_object_Position{ -1.5f, 0.0f, 0.0f }, m_rotation_Speed{ DEFAULT_ROTATION_SPEED }, m_Hexa_Vertex_Array_Object{ 0 }, m_num_faces_hexa{ 0 }, m_render_Polygon_Mode{ 0 }
{
	cout << "Constructor: Hexa_World(int window_width, int window_height)" << endl;
}

Hexa_World::Hexa_World() : Hexa_World(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT){}

Hexa_World::~Hexa_World()
{
	cout << "Destructor: ~Hexa_World()" << endl;
	if (m_texture_ID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_texture_ID);
	}

	if (m_Hexa_Vertex_Array_Object > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_Hexa_Vertex_Array_Object);
	}
}

void Hexa_World::run()
{
	if (canRun())
	{
		if (getGameWindow()->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
		{
			initialize();
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* Esto solo es para que al actualizarse el objeto gire*/
void Hexa_World::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}
	double degreesToRotate = 0.0;

	m_current_Delta_Time = deltaTime;

	degreesToRotate = m_rotation_Speed * (deltaTime / 1000.0);

	m_object_Rotation += degreesToRotate;

	while (m_object_Rotation > 360.0)
	{
		m_object_Rotation -= 360.0;
	}
	if (m_object_Rotation < 0.0)
	{
		m_object_Rotation = 0.0;
	}
}

/* */
void Hexa_World::initialize()
{
	isLoaded = false;
	//Buscamos el archivo JSON para leer
	ifstream file_json("hexgrid_cfg.json", ifstream::binary);
	file_json >> obj_json;

	//Ahora debo separar los datos importantes que son cuatro y guardarlos en los miembros de la clase
	num_cols = obj_json["HexGrid"]["numCols"];
	num_rows = obj_json["HexGrid"]["numRows"];
	cell_size = obj_json["HexGrid"]["cellSize"];

	if (obj_json["HexGrid"]["orientation"] == "pointy")
	{
		pointy_or_flat = true; //POINTY
	}
	else
	{
		pointy_or_flat = false; //FLAT
	}

	m_color_Model_Shader_Id = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_color_Model_Shader_Id == 0)
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	m_textured_Model_Shader_Id = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_textured_Model_Shader_Id == 0)
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	wstring wresourceFilenameTexture;
	string resourceFilenameTexture;

	if (!CWideStringHelper::GetResourceFullPath(MC_LEAVES_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	m_texture_ID = 0;
	if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &m_texture_ID, getOpenGLRenderer()))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	m_initialized = true;
	
	obj_grid.initialize(num_cols, num_rows, cell_size, pointy_or_flat);
	carga_openGL();
}

void Hexa_World::carga_openGL()
{

	isLoaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(&m_color_Model_Shader_Id, &m_Hexa_Vertex_Array_Object, obj_grid.m_vertex_data,
		6,	
		obj_grid.m_normal_Data,
		4,
		obj_grid.m_vertex_UVs,
		6,
		obj_grid.m_face_Indices,
		4,
		obj_grid.m_face_Normal_Indices,
		4,
		obj_grid.m_face_Indices,
		4
	);

	if (!isLoaded)
	{
		if (m_Hexa_Vertex_Array_Object > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_Hexa_Vertex_Array_Object);
			m_Hexa_Vertex_Array_Object = 0;
		}
	}

}


//Importante para mandar a llamar el hex_grid
void Hexa_World::render()
{
	CGameMenu *menu = getMenu();
	
	if (menu != NULL && menu->isInitialized() && menu->isActive())
	{
	}
	else
	{
		CVector3 posicion = { 0.0f,0.0f,0.0f };
		unsigned int m_caras_hexa = 4;
		w_p = sqrtf(3) * cell_size;
		h_p = 2 * cell_size;
		h_p = h_p * 0.75;
		float color[3] = { 41.0f, 255.0f, 218.0f }; 
		unsigned int noTexture = 0;
	
		double totalDegreesRotatedRadians = m_object_Rotation * 3.1459 / 180.0;

		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, m_object_Position);
	
		if (m_Hexa_Vertex_Array_Object > 0 && m_caras_hexa > 0)
		{
			CVector3 pos2 = m_object_Position;
			pos2 += CVector3(3.0f, 0.0f, 0.0f);
			MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation((float)totalDegreesRotatedRadians, pos2);
	
			getOpenGLRenderer()->renderObject
			(
				&m_textured_Model_Shader_Id,
				&m_Hexa_Vertex_Array_Object,
				&noTexture,
				m_caras_hexa,
				color,
				NULL,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}
		index++;
		if (pointy_or_flat == true)//Pointy
		{
			if (index < num_cols)
			{
				posicion = posicion + CVector3(w_p, 0.0f, 0.0f);
			}
			else if (index < num_rows)
			{
				posicion = posicion + CVector3((w_p / 2), 0.0f, h_p);
		
			}
		}
	}
}