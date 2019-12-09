#include "..\Include\AABB_2D.h"

void AABB_2D::getCorners(CVector3 * _corner){

	for (int i = 0; i < 4; i++) {

		_corner[i] = m_corners[i];
	}
}

void AABB_2D::setCorners(CVector3 * _corner){

	for (int i = 0; i < 4; i++) {

		m_corners[i] = _corner[i];
	}
}

bool AABB_2D::PointsInSide(CVector3 * _corner){
	return false;
}

AABB_2D::AABB_2D(){}
AABB_2D::AABB_2D(CVector3 * _corners){}