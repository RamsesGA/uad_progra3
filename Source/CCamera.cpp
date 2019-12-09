#include "..\Include\CCamera.h"

CCamera::CCamera(CVector3 _m_worldUp, CVector3 _m_eyePos, CVector3 _m_lookAt, int _m_frameBufferWidth, int _m_frameBufferHeight, float _m_fieldOfView, float _m_nearPlane, float _m_farPlane){

	m_worldUp = _m_worldUp;
	m_eyePos = _m_eyePos;
	m_lookAt = _m_lookAt;
	m_frameBufferWidth = _m_frameBufferWidth;
	m_frameBufferHeight = _m_frameBufferHeight;
	m_fieldOfView = _m_fieldOfView;
	m_nearPlane = _m_nearPlane;
	m_farPlane = _m_farPlane;
}

void CCamera::updateFrameBufferSize(int w, int h){}

void CCamera::update(float deltaTime){}

void CCamera::moveForward(float deltaMove){}

void CCamera::moveUp(float deltaMove){}

void CCamera::strafe(float deltaMove){}

void CCamera::orbitAroundLookAt(float deltaMove){}

bool CCamera::isAABB_2DVisible(AABB_2D & _aabb2D)
{
	if (!m_cacheValid)
	{
		recalculateViewMatrix();
		recalculateViewFrustrum();
	}

	// If the AABB is either completely visible (all corners inside the frustum), or intersecting (at least one corner inside the frustum)
	return (
		m_viewFrustum.isAABBVisible(_aabb2D) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT ||
		m_viewFrustum.isAABBVisible(_aabb2D) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE
		);
}

/*
bool CCamera::isAABBVisible(AABB & aabb)
{
	if (!m_cacheValid)
	{
		recalculateViewMatrix();
		recalculateViewFrustrum();
	}

	// If the AABB is either completely visible (all corners inside the frustum), or intersecting (at least one corner inside the frustum)
	return (
		m_viewFrustum.isAABBVisible(aabb) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT ||
		m_viewFrustum.isAABBVisible(aabb) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE
		);
}*/

void CCamera::recalculateViewMatrix(){}

void CCamera::recalculateProjectionMatrix(){}

void CCamera::recalculateViewFrustrum()
{
	// Avoid division by zero
	if (m_frameBufferHeight > 0)
	{
		// Geometric approach
		m_viewFrustum.update(
			m_eyePos,
			m_lookAt,
			m_up,
			m_right,
			m_nearPlane,
			m_farPlane,
			m_fieldOfView,
			(float)m_frameBufferWidth / (float)m_frameBufferHeight);
	}
}