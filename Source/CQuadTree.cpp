#include "..\Include\CQuadTree.h"

void CQuadTree::Subdivide(AABB_2D _aabb){

	if (m_root == nullptr) {

		m_root = new CQuadTreeNode;
		m_root->SetCornersLimit(_aabb);
		//m_root->Subdivide(m_limit, _cells);
	}
}

bool CQuadTree::LoadGeometry(COpenGLRenderer * _render, unsigned int _colorModelShaderID){

	if (m_root != nullptr) {

		bool isLoaded = _render->allocateGraphicsMemoryForObject
		(
			&_colorModelShaderID,
			&m_root->m_vertexArrayObject,
			m_root->m_verticesNodo,
			4,
			m_root->m_normales,
			2,
			m_root->m_uvCaras,
			4,
			m_root->m_indices,
			2,
			m_root->m_indicesNormales,
			2,
			m_root->m_indices,
			2
		);

		return isLoaded;
	}
	return false;
}

CQuadTree::CQuadTree(){

	m_root = nullptr;
}
