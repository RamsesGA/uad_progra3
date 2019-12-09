#pragma once
#include "CVector3.h"

class AABB_2D
{
	private:
	
	public:
		//Metodos
		CVector3 m_corners[4];

		void 
		getCorners(CVector3 *_corner);
	
		void 
		setCorners(CVector3 *_corner);

		bool
		PointsInSide(CVector3 *_corner);
	
		//Constructor
		AABB_2D();
		AABB_2D(CVector3 *_corners);
};
