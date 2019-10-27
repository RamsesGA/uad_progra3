#pragma once
//Solo es necesario Globals, CApp, ../Dependencies/JSON/nlohmann/json.hpp, ../Include/Hexa_Grid.h, 
#include "Globals.h"
#include "CApp.h"
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "../Include/Hexa_Grid.h"

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
		Hexa_World *m_objLoader; //Variable para mandar a llamar la función para cargar un modelo obj

		json obj_json; //Objeto tipo json

		bool isLoaded; //Booleano para poder dar la dirección ID de las variables

		int cont; //Contador para el for
		unsigned int num_cols; //Cantidad de columnas
		unsigned int num_rows; //Cantidad de filas
		float cell_size; //Tamaño de la celda
		bool pointy_or_flat = false; //false es para el FLAT
		bool izq_o_dere = true; //false es para la izq

		Hexa_Grid obj_grid;//Objeto para mandar a llamar variables del .h

		//Vectores para moverme en el plano para el hexagrid
		CVector3 posicion = { 0.0f,0.0f,0.0f };
		CVector3 posicion_2 = { 0.0f,0.0f,0.0f };

		//FORMULA HEXA GRID POINTY
		float w = 0;
		float h = 0;

		unsigned int index = 0; //Contador para saber la cantidad exacta de figuras creadas

		void initialize(); //Función que manda a llamar "llamada_hex_grid"

		void llamada_hex_grid(); //Función que manda a llamar el "initialize" del hexa_grid
	
		void carga_openGL(); //Función dedicada para cargar los ID

		void render(); //Función vital para poder dibujar en pantalla
	





		void run();

		void update(double deltaTime);



		// Override on F2/F3
		void onF3(int mods);
		void moveCamera(float _direction);
};
