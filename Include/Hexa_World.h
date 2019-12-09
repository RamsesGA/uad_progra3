#pragma once
//Solo es necesario Globals, CApp, ../Dependencies/JSON/nlohmann/json.hpp, ../Include/Hexa_Grid.h, C3DModel 
#include "Globals.h"
#include "CApp.h"
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "../Include/Hexa_Grid.h"
#include "../Include/C3DModel.h"
#include "../Include/C3DModel_Obj.h"
#include "../Include/CAppObjLoader.h"

using json = nlohmann::json;

class Hexa_World : public CApp
{
	protected:
		bool initializeMenu() { return false; }

	private:

		bool m_initialized;

		int m_num_faces_hexa;

		int m_render_Polygon_Mode;

		double m_rotation_Speed;

		double m_object_Rotation;
	
		double m_current_Delta_Time;
	
		CVector3 m_object_Position;
	
		unsigned int m_texture_ID;
		
		unsigned int m_color_Model_Shader_Id;

		unsigned int m_textured_Model_Shader_Id;

		unsigned int m_Hexa_Vertex_Array_Object;

	public:
		// Constructors and destructor
		Hexa_World(int window_width, int window_height);
		Hexa_World();
		~Hexa_World();


		//Variables
		vector<C3DModel*> m_game_objects; //Variable para mandar a llamar la función para cargar un modelo obj

		CAppObjLoader loader_obj; 

		json obj_json; //Objeto tipo json

		bool isLoaded; //Booleano para poder dar la dirección ID de las variables

		int num_cols; //Cantidad de columnas
		int num_rows; //Cantidad de filas
		int index = 0; //Contador para saber la cantidad exacta de figuras creadas

		float cell_size; //Tamaño de la celda

		bool pointy_or_flat = false; //false es para el FLAT
		bool izq_o_dere = true; //false es para la izq

		//Variables para la formula del desplazamiento del Hexa Grid
		float w = 0.0f;
		float h = 0.0f;

		Hexa_Grid obj_grid;//Objeto para mandar a llamar variables del .h

		//Variables necesarias para poder dibujar los obj
		string objName = "";
		string objFilename = "";

		string obj_name_2 = "";
		int obj_row = 0;
		int obj_column = 0;
		float obj_scale = 0.0f;
		CVector3 obj_rotation = { 0.0f, 0.0f, 0.0f };
		
		vector <string> model_name; //variable para conocer el nombre clave del objeto

		C3DModel *model;

		//Vectores para moverme en el plano para el hexagrid
		CVector3 posicion = { 0.0f,0.0f,0.0f };
		CVector3 posicion_2 = { 0.0f,0.0f,0.0f };

		


		void initialize(); //Función que manda a llamar "llamada_hex_grid"

		void llamada_hex_grid(); //Función que manda a llamar el "initialize" del hexa_grid
	
		void carga_openGL(); //Función dedicada para cargar los ID

		void render(); //Función vital para poder dibujar en pantalla




		void run();

		void update(double deltaTime);



		// Override on F2/F3
		void onF2(int mods);
		void onF3(int mods);
		void moveCamera(float _direction);
		void onMouseMove(float deltaX, float deltaY);
};
