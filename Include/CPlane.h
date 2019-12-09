#pragma once

#ifndef CPLANE_H
#define CPLANE_H

#include "CVector3.h"

/*
*/
class CPlane
{
	private:
		// http://www.lighthouse3d.com/tutorials/maths/plane/
		// Plane equation :
		// Ax + By + Cz + D = 0
		// The values of A, B, C will be calculated from 3 points that are known to be on the plane
		// D will be calculated from A, B, C
	
		float A, B, C, D;
	
	public:
		CPlane();
		CPlane(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2);
		~CPlane();
	
		// The distance to a point will be calculated from substituting the given point in the plane equation
		float 
		distanceToPoint(CVector3 &point);
	
		// Project point q onto this plane and return the projected point
		CVector3 projectPoint(CVector3 &point);
	
		// Method to update the points of a plane
		void 
		setPoints(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2);
};

#endif