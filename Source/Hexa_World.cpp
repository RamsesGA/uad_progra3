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
#include "../Include/CAppObjLoader.h"

//------------------------------------------------------------------------------------------------------------
Hexa_World::Hexa_World(int window_width, int window_height) : CApp(window_width, window_height), m_current_Delta_Time{ 0.0 }, m_object_Rotation{ 0.0 }, m_object_Position{ -1.5f, 0.0f, 0.0f }, m_rotation_Speed{ DEFAULT_ROTATION_SPEED }, m_Hexa_Vertex_Array_Object{ 0 }, m_num_faces_hexa{ 0 }, m_render_Polygon_Mode{ 0 }
{
	cout << "Constructor: Hexa_World(int window_width, int window_height)" << endl;
}

//------------------------------------------------------------------------------------------------------------
Hexa_World::Hexa_World() : Hexa_World(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT) {}

//------------------------------------------------------------------------------------------------------------
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


//Cositas extra

//------------------------------------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------------------------------
void Hexa_World::moveCamera(float _direction)
{
	if (getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->simpleCameraZoom(_direction);
	}
}

void Hexa_World::onMouseMove(float deltaX, float deltaY)
{
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		m_object_Position.getValues(currPos);
		currPos[0] += moveX;
		currPos[2] += moveZ;
		m_object_Position.setValues(currPos);
	}
}

//------------------------------------------------------------------------------------------------------------
void Hexa_World::run()
{
	if (canRun())
	{
		if (getGameWindow()->create(CAPP_PROGRA3_HEXGRID_WINDOW_TITLE))
		{
			cont = 0;//Le damos el valor por default a cont, del .h
			initialize();
			getOpenGLRenderer()->setClearScreenColor(119.0f, 136.0f, 153.f);
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




/* Aquí inicia lo mio ^-^ */
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
	
	llamada_hex_grid();
}

//Función solo para poder mandar a inicializar los puntos y vertices del hexágono
void Hexa_World::llamada_hex_grid()
{
	obj_grid.initialize(num_cols, num_rows, cell_size, pointy_or_flat, posicion);
	carga_openGL();
}

//Función para poder cargar los IDs
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

//Importante para mandar a imprimir el hexa_grid
void Hexa_World::render()
{
	CGameMenu *menu = getMenu();
	posicion = CVector3(0.0f, 0.0f, 0.0f);
	posicion_2 = CVector3(0.0f, 0.0f, 0.0f);

	if (menu != NULL && menu->isInitialized() && menu->isActive()){}
	else
	{
		w = sqrtf(3) * cell_size;
		h = 2 * cell_size;
		float color[3] = { 41.0f, 255.0f, 218.0f }; 
		unsigned int noTexture = 0;
	
		for (cont; cont < num_cols * num_rows; cont++)
		{
			if (m_Hexa_Vertex_Array_Object > 0 && obj_grid.numFaces > 0)
			{
				MathHelper::Matrix4 modelMatrix2 = MathHelper::SimpleModelMatrixRotationTranslation(0, posicion);

				getOpenGLRenderer()->renderObject
				(
					&m_textured_Model_Shader_Id,
					&m_Hexa_Vertex_Array_Object,
					&noTexture,
					obj_grid.numFaces,
					color,
					&modelMatrix2,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);
			}
			index++;
			//Pointy
			if (pointy_or_flat == true)
			{
				if (index < num_cols)//Condición para poder moverse en X
				{
					posicion = posicion + CVector3(w, 0.0f, 0.0f);
				}
				else
				{
					if (izq_o_dere == false)//Se moverá a la izquierda, bajando una pos y avanzando después en X
					{
						posicion_2 = posicion_2 + CVector3(-(w / 2), 0.0f, (h * 0.75f));
						posicion = posicion_2;
						izq_o_dere = true; //Para poder bajar y continuar de manera correcta
						index = 0; //Vital reiniciar, para seguir imprimiendo
					}
					else if (izq_o_dere == true)//Se moverá a la derecha, bajando una pos y avanzando después en X
					{
						posicion_2 = posicion_2 + CVector3((w / 2), 0.0f, (h * 0.75f));
						posicion = posicion_2;
						izq_o_dere = false; //Para poder bajar y continuar de manera correcta
						index = 0; //Vital reiniciar, para seguir imprimiendo
					}
					
				}
			}
			//Flat
			else if (pointy_or_flat == false)
			{
				if (index < num_rows)//Condición para poder moverse en X
				{
					posicion = posicion + CVector3(0.0f, 0.0f, w);
				}
				else
				{
					if (izq_o_dere == false)//Se moverá a la Arriba
					{
						posicion_2 = posicion_2 + CVector3((h * 0.75f), 0.0f, (w / 2));
						posicion = posicion_2;
						izq_o_dere = true; //Para poder bajar y continuar de manera correcta
						index = 0; //Vital reiniciar, para seguir imprimiendo
					}
					else if (izq_o_dere == true)//Se moverá a la Abajo
					{
						posicion_2 = posicion_2 + CVector3((h * 0.75f), 0.0f, -(w / 2));
						posicion = posicion_2;
						izq_o_dere = false; //Para poder bajar y continuar de manera correcta
						index = 0; //Vital reiniciar, para seguir imprimiendo
					}
				}
			}
		}
	}
}

void Hexa_World::onF2(int mods)
{
	loader_obj.onF2(mods);
}