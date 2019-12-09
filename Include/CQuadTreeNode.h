#pragma once
#include "COpenGLRenderer.h"
#include "Centros_Hexa.h"
#include "CCamera.h"
#include "AABB_2D.h"
#include <iostream>
#include <vector>


class CQuadTreeNode
{
	private:

		CQuadTreeNode *m_h1;
		CQuadTreeNode *m_h2;
		CQuadTreeNode *m_h3;
		CQuadTreeNode *m_h4;
	
		std::vector <Centers*> m_data;
	
		AABB_2D m_bound;

	public:

		unsigned int m_vertexArrayObject;
		float m_verticesNodo[12];
		float m_uvCaras[4];
		unsigned short m_indices[6];
		float m_normales[6];
		unsigned short m_indicesNormales[6];

		vector< vector< Centers > > m_centerData;

		void 
		Subdivide(AABB_2D _paramBounds, int _limit, std::vector< Centers*> *_cells);

		void
		Subdivide(AABB_2D _paramBounds, int _limits, vector< vector< Centers > > _centerData);

		void
		render(COpenGLRenderer *_render, unsigned int &_colorModelShaderId);

		void
		SetCornersLimit(AABB_2D _limits);



		CQuadTreeNode();
		~CQuadTreeNode();
};