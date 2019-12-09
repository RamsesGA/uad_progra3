#include "../Include/AABB.h"

//Constructor para almacenar los puntos de las esquinas
AABB::AABB(CVector3 *_corners)
{
	m_corners[0] = *_corners;
	_corners++;
	m_corners[1] = *_corners;
	_corners++;
	m_corners[2] = *_corners;
	_corners++;
	m_corners[3] = *_corners;
	_corners++;

	setCorners(_corners);
}

//Está función sirve para poder recibir los datos
void AABB::setCorners(CVector3 * _corner)
{
	m_corners[0] = *_corner;
	_corner++;
	m_corners[1] = *_corner;
	_corner++;
	m_corners[2] = *_corner;
	_corner++;
	m_corners[3] = *_corner;
	_corner++;
}

//Está función sirve para regresar los datos
void AABB::getCorners(CVector3 * _corner)
{
	*_corner = m_corners[0];
	_corner++;
	*_corner = m_corners[1];
	_corner++;
	*_corner = m_corners[2];
	_corner++;
	*_corner = m_corners[3];
	_corner++;
}

AABB::AABB(){}

AABB::~AABB(){}
