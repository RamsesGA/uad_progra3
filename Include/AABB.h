#pragma once
#include "../Include/CVector3.h"

class AABB
{
	private:
		CVector3 m_corners[4];
	
	public:
		//Metodos
		int m_points[8];
	
	
		void
		getCorners(CVector3 *_corner);
	
		void
		setCorners(CVector3 *_corner);
		
		//Constructor
		AABB();
		AABB(CVector3 *_corners);
	
		//Destructor
		~AABB();
	
};