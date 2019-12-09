#pragma once

#ifndef CFRUSTUM_H
#define CFRUSTUM_H

#include "MathHelper.h"
#include "AABB.h"
#include "AABB_2D.h"
#include "CVector3.h"
#include "CPlane.h"


class CFrustum
{
private:

	enum FRUSTUM_PLANE_ID
	{
		TOP_PLANE = 0,
		BOTTOM_PLANE,
		LEFT_PLANE,
		RIGHT_PLANE,
		NEAR_PLANE,
		FAR_PLANE
	};

	CPlane m_planes[6];

	void
	setPlanePoints(
		CVector3 farTopLeft,
		CVector3 farTopRight,
		CVector3 farBottomLeft,
		CVector3 farBottomRight,
		CVector3 nearTopLeft,
		CVector3 nearTopRight,
		CVector3 nearBottomLeft,
		CVector3 nearBottomRight
	);

public:
	//Variables


	enum class FRUSTUM_VISIBILITY_TEST_RESULT{

		OUTSIDE = 0,
		INTERSECT,
		INSIDE
	};

	CFrustum();
	~CFrustum();

	//FRUSTUM_VISIBILITY_TEST_RESULT isAABBVisible(AABB &aabb);
	FRUSTUM_VISIBILITY_TEST_RESULT isAABBVisible(AABB_2D &aabb);
	FRUSTUM_VISIBILITY_TEST_RESULT isPointVisible(CVector3 &point);

	void
	update(CVector3 eyePos, CVector3 lookAtPos, CVector3 upVector, CVector3 right, float nearD, float farD, float fov, float aspectRatio);
	void 
	update(MathHelper::Matrix4 *projMatrix, MathHelper::Matrix4 *viewMatrix);
};

#endif