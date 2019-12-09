#pragma once

#include "CVector3.h"
#include "AABB.h"
#include "CFrustrum.h"


class CCamera
{
	private:
		CVector3 m_worldUp, //(constructor) 0, 1, 0
				 m_eyePos, //(constructor)
				 m_lookAt, //(constructor)
				 m_up,
				 m_right;
	
		int m_frameBufferWidth, //(constructor)
			m_frameBufferHeight; //(constructor)
	
		float   m_fieldOfView, //(constructor)
				m_nearPlane, //(constructor)
				m_farPlane; //(constructor)

		bool m_cacheValid;
		CFrustum m_viewFrustum;
	
	
	public:

		CCamera(CVector3 _m_worldUp,
				CVector3 _m_eyePos,
				CVector3 _m_lookAt,
				int _m_frameBufferWidth,
				int _m_frameBufferHeight,
				float _m_fieldOfView,
				float _m_nearPlane,
				float _m_farPlane);

		CCamera() {}

		void 
		updateFrameBufferSize(int w, int h);
	
		void
		update(float deltaTime);
	
		void
		moveForward(float deltaMove); //Flechas izquierda y derecha
	
		void
		moveUp(float deltaMove); //Flechas arriba y abajo
	
		void 
		strafe(float deltaMove); //Letras A, D
	
		void 
		orbitAroundLookAt(float deltaMove); //Puntos extra al lograrlo
	
		bool 
		isAABB_2DVisible(AABB_2D &_aabb2D);
	
	private: //Estás funciones no las dará el profesor
		void 
		recalculateViewMatrix();
		void 
		recalculateProjectionMatrix();
		void 
		recalculateViewFrustrum();
};