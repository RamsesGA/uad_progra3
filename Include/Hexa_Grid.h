#pragma once
#include "Globals.h"
#include "../Include/CVector3.h"
#include "../Include/CTextureLoader.h"
#include "../Include/COpenGLRenderer.h"

#ifndef HEXA_GRID_H
#define HEXA_GRID_H

class Hexa_Grid
{
	public:
		// Constructors and destructor
		Hexa_Grid();
		~Hexa_Grid();


		//Variables vitales para poder usar en Hexa_World
		float m_vertex_data[18];
		float m_vertex_UVs[12];
		float m_normal_Data[12];
		unsigned short m_face_Indices[12];
		unsigned short m_face_Normal_Indices[12];

		unsigned int numFaces; //Variable para indicar el número de caras
	
		//Función para poder recibir los datos importantes desde "llamada_hex_grid"
		void initialize(unsigned int _num_cols, unsigned int _num_rows, float _cell_size, bool _p_or_f, CVector3 _center);

		//Función para poder declarar los puntos
		CVector3 hexa_pointy_points(CVector3 _center, int _index, float _cell_size, bool _p_or_f);
		//Función para poder conectar los puntos y dar valores a los miembros
		void structure_hexa(bool _p_or_f);
};

#endif // !HEXA_GRID_H