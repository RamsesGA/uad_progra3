#include "../Include/CQuadTreeNode.h"


void CQuadTreeNode::Subdivide(AABB_2D _parentBounds, int _limit, std::vector<Centers*>* _cells){

	int triCount = 0;

	for (int i = 0; i < _cells->size(); i++) {

		//triCount += _cells[i]->getTriangleCount();
	}
	if (_cells->size() == 1 || triCount < _limit) {

		for (int i = 0; i < _cells->size(); i++) {

			//m_data.push_back(_cells);
		}

		m_h1 = m_h2 = m_h3 = m_h4 = nullptr;
	}
	else {

		CVector3 corners[4];
		AABB_2D childBounds[4];

		for (int i = 0; i < 4; i++) {

			//corners[i] = _parentBounds.getCorners(i);
		}

		corners[1].X /= 2;
		corners[3].X /= 2;
		corners[2].Z /= 2;
		corners[3].Z /= 2;

		childBounds[0].setCorners(corners);

		for (int i = 0; i < 4; i++) {

			//corners[i] = _parentBounds.getCorners(i);
		}

		corners[0].X /= 2; 
		corners[2].X /= 2;
		corners[2].Z /= 2;
		corners[3].Z /= 2;

		childBounds[1].setCorners(corners);

		for (int i = 0; i < 4; i++) {

			//corners[i] = _parentBounds.getCorners(i);
		}

		corners[0].Z /= 2;
		corners[1].X /= 2;
		corners[1].Z /= 2;
		corners[3].X /= 2;

		childBounds[2].setCorners(corners);

		for (int i = 0; i < 4; i++) {

			//corners[i] = _parentBounds.getCorners(i);
		}

		corners[0].X /= 2;
		corners[2].X /= 2;
		corners[0].Z /= 2;
		corners[1].Z /= 2;

		childBounds[3].setCorners(corners);

		m_h1 = new CQuadTreeNode();
		m_h1->SetCornersLimit(childBounds[0]);
		m_h2 = new CQuadTreeNode();
		m_h2->SetCornersLimit(childBounds[1]);
		m_h3 = new CQuadTreeNode();
		m_h3->SetCornersLimit(childBounds[2]);
		m_h4 = new CQuadTreeNode();
		m_h4->SetCornersLimit(childBounds[3]);

		std::vector <Centers*> childCells[4];

		for (int i = 0; i < _cells->size(); i++) {

			//Centers cell = _cells->at(i);
			CVector3 corners[4];

			//Obtener esquinas en la celda
			//if (childBounds[0]
		}
	}
}

void CQuadTreeNode::Subdivide(AABB_2D _paramBounds, int _limits, vector<vector<Centers>> _centerData){}

void CQuadTreeNode::render(COpenGLRenderer *_render, unsigned int &_colorModelShaderI){

	CVector3 tempo{ 0.0f,0.0f,0.0f };
	float color[3] = { 255.0f, 255.0f, 255.0f };
	MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(0, tempo);
	
	_render->renderObject
	(
		&_colorModelShaderI,
		&m_vertexArrayObject,
		0,
		2,
		color,
		&modelMatrix,
		COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
		false
	);
}

void CQuadTreeNode::SetCornersLimit(AABB_2D _limits){

	m_bound = _limits;

	//Punto 0
	m_verticesNodo[0] = m_bound.m_corners[0].getX();
	m_verticesNodo[1] = m_bound.m_corners[0].getY();
	m_verticesNodo[2] = m_bound.m_corners[0].getZ();

	//Punto 1
	m_verticesNodo[3] = m_bound.m_corners[1].getX();
	m_verticesNodo[4] = m_bound.m_corners[1].getY();
	m_verticesNodo[5] = m_bound.m_corners[1].getZ();

	//Punto 2
	m_verticesNodo[6] = m_bound.m_corners[2].getX();
	m_verticesNodo[7] = m_bound.m_corners[2].getY();
	m_verticesNodo[8] = m_bound.m_corners[2].getZ();

	//Punto 3
	m_verticesNodo[9] = m_bound.m_corners[3].getX();
	m_verticesNodo[10] = m_bound.m_corners[3].getY();
	m_verticesNodo[11] = m_bound.m_corners[3].getZ();

	//Posicion 1
	m_uvCaras[0] = 0.0f;
	m_uvCaras[1] = 0.0f;

	//Posición 2
	m_uvCaras[2] = 0.0f;
	m_uvCaras[3] = 0.0f;

	//Primera cara
	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 3;

	//Segunda cara
	m_indices[3] = 0;
	m_indices[4] = 3;
	m_indices[5] = 2;

	m_normales[0] = 0.0f;
	m_normales[1] = 0.0f;
	m_normales[2] = 0.0f;
	m_normales[3] = 0.0f;
	m_normales[4] = 0.0f;
	m_normales[5] = 0.0f;

	m_indicesNormales[0] = 0;
	m_indicesNormales[1] = 0;
	m_indicesNormales[2] = 0;
	m_indicesNormales[3] = 0;
	m_indicesNormales[4] = 0;
	m_indicesNormales[5] = 0;
}

CQuadTreeNode::CQuadTreeNode(){}
CQuadTreeNode::~CQuadTreeNode(){}