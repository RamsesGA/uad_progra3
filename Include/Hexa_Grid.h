#pragma once
#include "Globals.h"
#include <vector>
#include "../Include/CVector3.h"
#include "../Include/CTextureLoader.h"
#include "../Include/COpenGLRenderer.h"
#include "../Include/Centros_Hexa.h"

#ifndef HEXA_GRID_H
#define HEXA_GRID_H

using std::vector; 

class Hexa_Grid
{
	public:
		// Constructors and destructor
		Hexa_Grid();
		~Hexa_Grid();

		//Variables para la formula del desplazamiento del Hexa Grid
		float w = 0.0f;
		float h = 0.0f;

		//Variables vitales para poder usar en Hexa_World
		float m_vertex_data[18];
		float m_vertex_UVs[12];
		float m_normal_Data[12];

		unsigned short m_face_Indices[12];
		unsigned short m_face_Normal_Indices[12];
		unsigned int numFaces; //Variable para indicar el número de caras

		int obj_index = 0; //Para que la celda sepa cual de los modelos en vector<C3DModel*> m_gameobjects es el que debe cargar

		bool m_hasModel = false; //indica si tiene un objeto la celda
	
		vector <vector <Centers>> vec_center_hexa; //Vector de vectores para poder almacenar el centro en X / Z del hexágono generado

		//Función para poder recibir los datos importantes desde "llamada_hex_grid"
		void initialize(unsigned int _num_cols, unsigned int _num_rows, float _cell_size, bool _p_or_f, CVector3 _center);

		//Función para poder conectar los puntos y dar valores a los miembros
		void structure_hexa(bool _p_or_f);

		//Función para poder declarar los puntos
		CVector3 hexa_points(CVector3 _center, int _index, float _cell_size, bool _p_or_f);
};

#endif // !HEXA_GRID_H