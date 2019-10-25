#pragma once
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
		//Variables
		json obj_json;

		bool isLoaded;

		unsigned int num_cols;
		unsigned int num_rows;
		float cell_size;
		bool pointy_or_flat = false; //false es para el FLAT
		Hexa_Grid obj_grid;


		//FORMULA HEXA GRID POINTY
		float w_p = 0;
		float h_p = 0;
		unsigned int index = 0;

		// Constructors and destructor
		Hexa_World(int window_width, int window_height);
		Hexa_World();
		~Hexa_World();
	
		void initialize();
	
		void carga_openGL();

		void update(double deltaTime);
	
		void run();
	
		void render();
	
		// Override on F2/F3
		void onF3(int mods);
		void moveCamera(float _direction);
};
