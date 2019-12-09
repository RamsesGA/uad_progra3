#include "../stdafx.h"
#include "../Include/CPlane.h"
#include "../Include/CVector3.h"
#include "../Include/MathHelper.h"

/*
*/
CPlane::CPlane() :
	CPlane(CVector3(-1.0f, 1.0f, 0.0f), CVector3(1.0f, 1.0f, 0.0f), CVector3(1.0f, -1.0f, 0.0f))
{
}

/*
  Construct a plane from 3 points that are on the plane (COUNTER-CLOCK WISE ORDER!, otherwise the normal will be inverted... srsly... ??)
*/
CPlane::CPlane(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2)
{
	setPoints(pt0, pt1, pt2);
}

/*
*/
void CPlane::setPoints(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2)
{
	CVector3 v = pt1 - pt0;
	CVector3 u = pt2 - pt0;
	CVector3 normal = v.cross(u);
	normal.normalize();

	A = normal.X;
	B = normal.Y;
	C = normal.Z;

	// To compute the value of D we just use the equation above, hence -D = Ax + By + Cz. 
	// Replacing (x,y,z) for a point in the plane (for instance p0), we get D = ? n . p0 (dot product).
	normal *= -1.0f;
	D = normal.dot(pt0);
}

/*
*/
CPlane::~CPlane()
{
}

/*
*/
float CPlane::distanceToPoint(CVector3 &point)
{
	// dist = A*rx + B*ry + C*rz + D = n . r  + D
	CVector3 planeNormal;
	planeNormal.X = A;
	planeNormal.Y = B;
	planeNormal.Z = C;

	// Actually the distance is the absolute value of dist, but the sign of dist (if it is zero is on the plane) also gives information as to which side of the plane is point r. 
	// If the sign is positive then the point is on the side that agrees with the normal n, otherwise it is on the other side.
	return (planeNormal.dot(point)) + D;
}

/*
*/
CVector3 CPlane::projectPoint(CVector3 &point)
{
	CVector3 projectedPoint = CVector3::ZeroVector();

	// http://www.lighthouse3d.com/tutorials/maths/plane/
	// projectedPoint = sourcePoint - signedDistance * normal

	CVector3 planeNormal;
	planeNormal.X = A;
	planeNormal.Y = B;
	planeNormal.Z = C;

	float signedDistance = distanceToPoint(point);
	planeNormal *= signedDistance;

	projectedPoint = point - planeNormal;

	return projectedPoint;
}