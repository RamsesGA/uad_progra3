#pragma once
#include "../Include/CQuadTreeNode.h"
#include "Centros_Hexa.h"
#include "AABB_2D.h"
#include <vector>

class CQuadTree
{
	private:

		

	public:

		int m_limit;
		CQuadTreeNode *m_root;
		void
		Subdivide(AABB_2D _aabb);

		bool
		LoadGeometry(COpenGLRenderer *_render, unsigned int _colorModelShaderID);

		CQuadTree();
};