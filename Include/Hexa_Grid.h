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
		float m_vertex_data[18];
		float m_vertex_UVs[12];
		float m_normal_Data[12];

		
		unsigned short m_face_Indices[12];
		unsigned short m_face_Normal_Indices[12];

		unsigned int numFaces;

		// Constructors and destructor
		Hexa_Grid();
		~Hexa_Grid();
	
		void initialize(unsigned int _num_cols, unsigned int _num_rows, float _cell_size, bool _p_or_f);

		CVector3 hexa_pointy_points(CVector3 _center, int _index, float _cell_size);
		void structure_hexa();
};

#endif // !HEXA_GRID_H