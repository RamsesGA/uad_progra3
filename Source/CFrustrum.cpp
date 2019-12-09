#include "../stdafx.h"
#include "../Include/CFrustrum.h"

/*
*/
CFrustum::CFrustum(){}

/*
*/
CFrustum::~CFrustum(){}

/*
There are different ways to compute the frustum information.

  + A geometric approach takes the camera position and lookAt vectors, as well as the near and far planes and the fov and aspect ratio.
	The frustum points in world space are computed as a function of these variables.
	https://cgvr.cs.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
	-----------------------------------------------------------------------------------------------------------------------------------

	The apex of the pyramid is the position of the camera (px,py,pz). A view ray can be computed with direction d = l - p, where l = (lx,ly,lz) and origin p = (px,py,pz).
	The near and far planes are perpendicular to the view ray and are placed at distances nearDist and farDist.
	The rectangular boundaries of the view frustum in those planes, near and far, have dimensions that are a function of the distance and the fov (vertical field of view),
	and ratio (ratio between the horizontal and vertical fields of view).

	The height and width of the rectangular boundary on the near plane are defined as follows:

	Hnear = 2 * tan(fov / 2) * nearDist
	Wnear = Hnear * ratio

	The same reasoning can be applied to the far plane:

	Hfar = 2 * tan(fov / 2) * farDist
	Wfar = Hfar * ratio

	The planes are defined such that the normal points towards the inside of the view frustum. Testing if an object is inside the view frustum is performed
	by computing on which side of the plane the object resides. This can be done computing the signed distance from the point ot the plane.
	If it is on the side that the normal is pointing, i.e. the signed distance is positive, then it is on the right side of the respective plane.
	If an object is on the right side of all six planes then the object is inside the frustum.

  + Another approach is to compute the frustum using the projection matrix and the inverse of the movelview matrix.
	Hartmann/Gribbs method (http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf)
	---------------------------------------------------------------------------------------------------------------

*/

/*
  Geometric approach of extracting the frustum information
*/
void CFrustum::update(CVector3 eyePos, CVector3 lookAtPos, CVector3 upVector, CVector3 right, float nearDistance, float farDistance, float fov, float aspectRatio)
{
	/*
	eyePos   - the camera position
	forward  - a vector with the direction of the camera's view ray. In here it is assumed that this vector has been normalized
	nearDist - the distance from the camera to the near plane
	Hnear    - the "height" of the near plane
	Wnear    - the "width" of the near plane
	farDist  - the distance from the camera to the far plane
	Hfar     - the "height" of the far plane
	Wfar     - the "width" of the far plane
	*/

	// Calculate forward vector from eye to target
	CVector3 forwardVector = lookAtPos - eyePos;
	// Normalize vector
	forwardVector.normalize();

	float tanFov2 = tanf((fov * PI_OVER_180) / 2.0f);

	float Hnear = 2 * tanFov2 * nearDistance;
	float Wnear = Hnear * aspectRatio;

	float Hfar = 2 * tanFov2 * farDistance;
	float Wfar = Hfar * aspectRatio;

	CVector3 farCenter = eyePos + (forwardVector * farDistance);
	float    HFarOver2 = Hfar / 2.0f;
	float    WFarOver2 = Wfar / 2.0f;
	CVector3 upHFarOver2 = upVector * HFarOver2;
	CVector3 rightWFarOver2 = right * WFarOver2;

	// Compute the 4 corners of the frustum on the far plane
	CVector3 farTopLeft = farCenter + upHFarOver2 - rightWFarOver2;
	CVector3 farTopRight = farCenter + upHFarOver2 + rightWFarOver2;
	CVector3 farBottomLeft = farCenter - upHFarOver2 - rightWFarOver2;
	CVector3 farBottomRight = farCenter - upHFarOver2 + rightWFarOver2;

	CVector3 nearCenter = eyePos + (forwardVector * nearDistance);
	float    HNearOver2 = Hnear / 2.0f;
	float    WNearOver2 = Wnear / 2.0f;
	CVector3 upHNearOver2 = upVector * HNearOver2;
	CVector3 rightWNearOver2 = right * WNearOver2;

	// Compute the 4 corners of the frustum on the near plane
	CVector3 nearTopLeft = nearCenter + upHNearOver2 - rightWNearOver2;
	CVector3 nearTopRight = nearCenter + upHNearOver2 + rightWNearOver2;
	CVector3 nearBottomLeft = nearCenter - upHNearOver2 - rightWNearOver2;
	CVector3 nearBottomRight = nearCenter - upHNearOver2 + rightWNearOver2;

	// Set plane points
	setPlanePoints(
		farTopLeft, farTopRight, farBottomLeft, farBottomRight,
		nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight);
}

/*
   Hartmann/Gribbs method
   TO-DO
   (Use the other update() for now, should give the same results...)
*/
void CFrustum::update(MathHelper::Matrix4 *projMatrix, MathHelper::Matrix4 *viewMatrix)
{
	/*
	The easiest way I know of is to construct the view frustum in post-projective space (so-called NDC space), where the frustum is a cube,
	then transform it through the inverse view-projection matrix and construct the planes in world space.

	The frustum in NDC space runs from -1 to +1 along the x and y axes, and from -1 to +1 on z if you're in OpenGL, or 0 to 1 on z if you're in D3D.
	Build the eight corners of this cube (setting w = 1), then multiply them by the inverse view-projection matrix, and divide by w to convert back to regular 3D points.

	The result should be the frustum corners in world space, and you can then build the planes by using groups of 3 points and the usual formula for calculating a plane equation from 3 points.

	This general technique should work with any kind of view-projection matrix - perspective or orthographic, oblique or straight, any FoV or aspect ratio, etc. There are ways to simplify it by making
	some assumptions about the form of the matrix, but those tend to break if any of the assumptions are changed.
	*/

	/*

	TO-DO. To be able to code this method, a CVector4 class needs to be created

	// Compute the 4 corners of the frustum on the far plane
	CVector3 farTopLeft      = CVector4(-1.0f,  1.0f, -1.0f, 1.0f);
	CVector3 farTopRight     = CVector4( 1.0f,  1.0f, -1.0f, 1.0f);
	CVector3 farBottomLeft   = CVector4(-1.0f, -1.0f, -1.0f, 1.0f);
	CVector3 farBottomRight  = CVector4( 1.0f, -1.0f, -1.0f, 1.0f);

	// Compute the 4 corners of the frustum on the near plane
	CVector3 nearTopLeft     = CVector4(-1.0f,  1.0f, 1.0f, 1.0f);
	CVector3 nearTopRight    = CVector4( 1.0f,  1.0f, 1.0f, 1.0f);
	CVector3 nearBottomLeft  = CVector4(-1.0f, -1.0f, 1.0f, 1.0f);
	CVector3 nearBottomRight = CVector4( 1.0f, -1.0f, 1.0f, 1.0f);

	http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf
	https://stackoverflow.com/questions/12836967/extracting-view-frustum-planes-hartmann-gribbs-method
	comboMatrix = projection_matrix * Matrix4_Transpose(modelview_matrix)

	p_planes[0].a = comboMatrix._41 + comboMatrix._11;
	p_planes[0].b = comboMatrix._42 + comboMatrix._12;
	p_planes[0].c = comboMatrix._43 + comboMatrix._13;
	p_planes[0].d = comboMatrix._44 + comboMatrix._14;
	// Right clipping plane
	p_planes[1].a = comboMatrix._41 - comboMatrix._11;
	p_planes[1].b = comboMatrix._42 - comboMatrix._12;
	p_planes[1].c = comboMatrix._43 - comboMatrix._13;
	p_planes[1].d = comboMatrix._44 - comboMatrix._14;
	// Top clipping plane
	p_planes[2].a = comboMatrix._41 - comboMatrix._21;
	p_planes[2].b = comboMatrix._42 - comboMatrix._22;
	p_planes[2].c = comboMatrix._43 - comboMatrix._23;
	p_planes[2].d = comboMatrix._44 - comboMatrix._24;
	// Bottom clipping plane
	p_planes[3].a = comboMatrix._41 + comboMatrix._21;
	p_planes[3].b = comboMatrix._42 + comboMatrix._22;
	p_planes[3].c = comboMatrix._43 + comboMatrix._23;
	p_planes[3].d = comboMatrix._44 + comboMatrix._24;
	// Near clipping plane
	p_planes[4].a = comboMatrix._41 + comboMatrix._31;
	p_planes[4].b = comboMatrix._42 + comboMatrix._32;
	p_planes[4].c = comboMatrix._43 + comboMatrix._33;
	p_planes[4].d = comboMatrix._44 + comboMatrix._34;
	// Far clipping plane
	p_planes[5].a = comboMatrix._41 - comboMatrix._31;
	p_planes[5].b = comboMatrix._42 - comboMatrix._32;
	p_planes[5].c = comboMatrix._43 - comboMatrix._33;
	p_planes[5].d = comboMatrix._44 - comboMatrix._34;


	// Set plane points
	setPlanePoints(
		farTopLeft, farTopRight, farBottomLeft, farBottomRight,
		nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight); */
}

/*
	Compute the six planes. Need to pass the points in counter clock wise order to setPoints
*/
void CFrustum::setPlanePoints(
	CVector3 farTopLeft,
	CVector3 farTopRight,
	CVector3 farBottomLeft,
	CVector3 farBottomRight,
	CVector3 nearTopLeft,
	CVector3 nearTopRight,
	CVector3 nearBottomLeft,
	CVector3 nearBottomRight
)
{
	// Compute the six planes. Need to pass the points in counter clock wise order to setPoints
	m_planes[TOP_PLANE].setPoints(nearTopRight, nearTopLeft, farTopLeft);
	m_planes[BOTTOM_PLANE].setPoints(nearBottomLeft, nearBottomRight, farBottomRight);
	m_planes[LEFT_PLANE].setPoints(nearTopLeft, nearBottomLeft, farBottomLeft);
	m_planes[RIGHT_PLANE].setPoints(nearBottomRight, nearTopRight, farBottomRight);
	m_planes[NEAR_PLANE].setPoints(nearTopLeft, nearTopRight, nearBottomRight);
	m_planes[FAR_PLANE].setPoints(farTopRight, farTopLeft, farBottomLeft);
}

/*
https://cgvr.cs.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
http://old.cescg.org/CESCG-2002/DSykoraJJelinek/
*/

/*CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT CFrustum::isAABBVisible(AABB &aabb)
{
	CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT result = FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE;
	int visibleCorners = 0;
	int totalCorners = 8;

	// If any of the 8 corners of the AABB is INSIDE ALL PLANES of the frustum, then the AABB is at least partially visible
	for (int i = 0; i < totalCorners; ++i)
	{
		if (isPointVisible(aabb.m_points[i]) == FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE)
		{
			++visibleCorners;
		}
	}

	// No corners visible, AABB is very likely outside the frustum
	if (visibleCorners == 0)
	{
		result = FRUSTUM_VISIBILITY_TEST_RESULT::OUTSIDE;

		// 
		// Add an additional test in case the AABB is large enough that all points are outside the frustum but it's still intersecting the frustum
		/*
			    ____________
			 eye\          / Frustum
				 \        /
				  \      /
				   \    /
				  __\__/__
				 |   \/   | AABB
				 |________|
		*/
		/*int numCornersOutside = 0;

		// For each plane
		for (int i = 0; i < 6; ++i)
		{
			// Reset counter
			numCornersOutside = 0;

			// For each corner of the AABB
			for (int j = 0; j < totalCorners; ++j)
			{
				if (m_planes[i].distanceToPoint(aabb.m_points[j]) < 0.0f)
				{
					++numCornersOutside;
				}
			}

			// All corners are outside of THE SAME plane, it's safe to reject the AABB
			if (numCornersOutside == totalCorners)
			{
				result = FRUSTUM_VISIBILITY_TEST_RESULT::OUTSIDE;
				break;
			}
			else if (numCornersOutside > 0 && numCornersOutside < totalCorners)
			{
				// Some of the corners of the AABB are inside and some are outside of this plane
				// Set the result temporarily to INTERSECT but don't break the loop and check the next plane
				result = FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT;
			}
		}

		// This is still not the perfect solution, some AABB would still be reported to be INTERSECTING the frustum
	}
	// At least one corner visible but not all 8, AABB is intersecting the frustum
	else if (visibleCorners < totalCorners)
	{
		result = FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT;
	}

	return result;
}*/

/*
https://cgvr.cs.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
http://old.cescg.org/CESCG-2002/DSykoraJJelinek/
*/
CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT CFrustum::isAABBVisible(AABB_2D &aabb)
{
	CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT result = FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE;
	int visibleCorners = 0;
	int totalCorners = 4;

	// If any of the 4 corners of the AABB is INSIDE ALL PLANES of the frustum, then the AABB is at least partially visible
	for (int i = 0; i < totalCorners; ++i)
	{
		if (isPointVisible(aabb.m_corners[i]) == FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE)
		{
			++visibleCorners;
		}
	}

	// No corners visible, AABB is very likely outside the frustum
	if (visibleCorners == 0)
	{
		result = FRUSTUM_VISIBILITY_TEST_RESULT::OUTSIDE;

		// 
		// Add an additional test in case the AABB is large enough that all points are outside the frustum but it's still intersecting the frustum
		/*
		   ____________
		eye\          / Frustum
			\        /
			 \      /
			  \    /
			 __\__/__
			|   \/   | AABB
			|________|
		*/
		int numCornersOutside = 0;

		// For each plane
		for (int i = 0; i < 6; ++i)
		{
			// Reset counter
			numCornersOutside = 0;

			// For each corner of the AABB
			for (int j = 0; j < totalCorners; ++j)
			{
				if (m_planes[i].distanceToPoint(aabb.m_corners[j]) < 0.0f)
				{
					++numCornersOutside;
				}
			}

			// All corners are outside of THE SAME plane, it's safe to reject the AABB
			if (numCornersOutside == totalCorners)
			{
				result = FRUSTUM_VISIBILITY_TEST_RESULT::OUTSIDE;
				break;
			}
			else if (numCornersOutside > 0 && numCornersOutside < totalCorners)
			{
				// Some of the corners of the AABB are inside and some are outside of this plane
				// Set the result temporarily to INTERSECT but don't break the loop and check the next plane
				result = FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT;
			}
		}

		// This is still not the perfect solution, some AABB would still be reported to be INTERSECTING the frustum
	}
	// At least one corner visible but not all 8, AABB is intersecting the frustum
	else if (visibleCorners < totalCorners)
	{
		result = FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT;
	}

	return result;
}

/*
*/
CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT CFrustum::isPointVisible(CVector3 &point)
{
	CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT result = FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE;

	// If the point is INSIDE ALL planes of the frustum, it is visible
	for (int i = 0; i < 6; ++i)
	{
		if (m_planes[i].distanceToPoint(point) < 0.0f)
		{
			return FRUSTUM_VISIBILITY_TEST_RESULT::OUTSIDE;
		}
	}

	return result;
}