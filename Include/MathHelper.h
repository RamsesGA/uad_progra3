#pragma once

// These are some simple math helpers to enable the template to render a spinning cube. It is not a complete math library.
// You can replace this with your favorite math library that's suitable for your target platforms, e.g. DirectXMath or GLM.

#include <math.h>
#include <iostream>
#include <iomanip>
#include "CVector3.h"

namespace MathHelper
{
#define PI_OVER_180 3.14159f / 180.0f

	/*
     *
     */
	struct Matrix4
	{
		Matrix4()
		{
			m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
			m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		}

		Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		float m[4][4];
	};

	/*
	 * Identity Matrix
	 */
	inline static Matrix4 IdentityMatrix()
	{
		return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
            		   0.0f, 1.0f, 0.0f, 0.0f,
			           0.0f, 0.0f, 1.0f, 0.0f,
			           0.0f, 0.0f, 0.0f, 1.0f);
	}

	/*
	 * Transpose
	 */
	inline static Matrix4 Transpose(Matrix4 m1)
	{
		float m[4][4];

		// For each row
		for (int i = 0; i < 4; ++i)
		{
			// For each col
			for (int j = 0; j < 4; ++j)
			{
				m[j][i] = m1.m[i][j];
			}
		}

		return Matrix4(
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		);
	}

	/*
	 * 9.005 Are OpenGL matrices column-major or row-major?
	 *
     * For programming purposes, OpenGL matrices are 16-value arrays with base vectors laid out contiguously in memory. 
     * The translation components occupy the 13th, 14th, and 15th elements of the 16-element matrix, 
     * where indices are numbered from 1 to 16 as described in section 2.11.2 of the OpenGL 2.1 Specification.
	 *
     * Column-major versus row-major is purely a notational convention. 
     * Note that post-multiplying with column-major matrices produces the same result as pre-multiplying with row-major matrices. 
     * The OpenGL Specification and the OpenGL Reference Manual both use column-major notation. You can use any notation, 
     * as long as it's clearly stated.
	 *
     * Sadly, the use of column-major format in the spec and blue book has resulted in endless confusion 
     * in the OpenGL programming community. Column-major notation suggests that matrices are not laid out in memory 
     * as a programmer would expect.
	 *
	 * Rx = |    1     0     0    |
	 *      |    0   cos@ -sin@   |  
	 *      |    0   sin@  cos@   |
	 *
	 * Ry = |  cos@    0   sin@   |
	 *      |    0     1     0    |
	 *      | -sin@    0   cos@   |
	 *
	 * Rz = |  cos@ -sin@    0    |
	 *      |  sin@  cos@    0    |
	 *      |    0     0     1    |
	 *
	*/

	/*
	 * Rotate around X
	 */
	inline static Matrix4 RotAroundX(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: Remember this needs to be a transposed
		Matrix4 rX(
			1.0f, 0.0f,    0.0f,   0.0f,
			0.0f, cosine, -sine,   0.0f,
			0.0f, sine,    cosine, 0.0f,
			0.0f, 0.0f,    0.0f,   1.0f);

		return Transpose(rX);
	}

	/*
	 * Rotate around Y
	 */
	inline static Matrix4 RotAroundY(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: Remember this needs to be a transposed
		Matrix4 rY(
			cosine, 0.0f, sine,   0.0f,
			0.0f,   1.0f, 0.0f,   0.0f,
			-sine,  0.0f, cosine, 0.0f,
			0.0f,   0.0f, 0.0f,   1.0f);

		return Transpose(rY);
	}

	/*
	 * Rotate around Z
	 */
	inline static Matrix4 RotAroundZ(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: Remember this needs to be a transposed
		Matrix4 rZ(
			cosine, -sine,   0.0f, 0.0f,
			sine,    cosine, 0.0f, 0.0f,
			0.0f,    0.0f,   0.0f, 0.0f,
			0.0f,    0.0f,   0.0f, 1.0f);

		return Transpose(rZ);
	}

	/*
	 * Scale matrix
	 */
	inline static Matrix4 ScaleMatrix(float sx, float sy, float sz)
	{
		// Transposing this matrix looks the same
		return Matrix4(
			sx,    0.0f,  0.0f,  0.0f,
			0.0f,  sy,    0.0f,  0.0f,
			0.0f,  0.0f,  sz,    0.0f,
			0.0f,  0.0f,  0.0f,  1.0f);
	}

	/*
	 * Multiply a vector (position) with a matrix
	 */
	inline static CVector3 MatrixMultVector(Matrix4 m, CVector3 v)
	{
		// In order to multiply a matrix by a vector, the number of columns on the matrix needs 
		// to be the same number of components of the vector, so we need a Vec4 in this case
		// we'll add a 1 to the end of a Vec3
		CVector3 result = CVector3::ZeroVector();

		result.X = (m.m[0][0] * v.X) + (m.m[0][1] * v.Y) + (m.m[0][2] * v.Z) + (m.m[0][3]);
		result.Y = (m.m[1][0] * v.X) + (m.m[1][1] * v.Y) + (m.m[1][2] * v.Z) + (m.m[1][3]);
		result.Z = (m.m[2][0] * v.X) + (m.m[2][1] * v.Y) + (m.m[2][2] * v.Z) + (m.m[2][3]);
		float  w = (m.m[3][0] * v.X) + (m.m[3][1] * v.Y) + (m.m[3][2] * v.Z) + (m.m[3][3] /* * 1.0f */);

		// If W is not 1.0, normalize/divide each component by W to convert from homogeneous coordinates to cartesian coordinates
		if (w != 1.0f)
		{
			result.X /= w;
			result.Y /= w;
			result.Z /= w;
		}

		return result;
	}

	/*
	 * Translation matrix
	 */
	inline static Matrix4 TranslationMatrix(float tx, float ty, float tz)
	{
		// This needs to be transposed
		Matrix4 m1(
			1.0f, 0.0f, 0.0f, tx,
			0.0f, 1.0f, 0.0f, ty,
			0.0f, 0.0f, 1.0f, tz,
			0.0f, 0.0f, 0.0f, 1.0f);

		return Transpose(m1);
	}

	/*
	 * Multiply m1 * m2
	 * TRANSFORM HAPPENS IN REVERSE ORDER
	 * If m1 is a TRANSFORM and m2 is a SCALE, the scale is applied first
	 */
	inline static Matrix4 Multiply(Matrix4 m1, Matrix4 m2)
	{
		float result[4][4];
		float mult = 0.0f;

		// For each row
		for (int i = 0; i < 4; ++i)
		{
			for (int colCount = 0; colCount < 4; ++colCount)
			{
				mult = 0.0f;

				// for each column
				for (int j = 0; j < 4; ++j)
				{
					// Multiply the row of m1 times the column of m2
					mult += (m1.m[i][j] * m2.m[j][colCount]);
				}

				result[i][colCount] = mult;

			}
		}

		return Matrix4(
			result[0][0], result[0][1], result[0][2], result[0][3],
			result[1][0], result[1][1], result[1][2], result[1][3],
			result[2][0], result[2][1], result[2][2], result[2][3],
			result[3][0], result[3][1], result[3][2], result[3][3]
		);
	}

	/*
	 * Return a Orthographic Projection Matrix
	 */
	inline static Matrix4 OrthographicProjectionMatrix(
		const float &angleOfView,  // Angle of view in degrees
		const float &aspectRatio,  // Aspect ratio (width/height of the framebuffer, not the window)
		const float &near_plane,   // Near plane
		const float &far_plane)    // Far plane
	{
		float top = tanf(angleOfView * 0.5f * PI_OVER_180) * near_plane;
		float bottom = -top;
		float right = top * aspectRatio;
		float left = -right;

		// Remember to transpose
		Matrix4 ortho(
			2.0f  / (right - left),  0.0f,                    0.0f,                             -((right + left) /(right - left)),
			0.0f,                    2.0f  / (top - bottom),  0.0f,                             -((top + bottom) / (top - bottom)),
			0.0f,                    0.0f,                   -2.0f / (far_plane - near_plane), -((far_plane + near_plane) / (far_plane - near_plane)),
			0.0f,                    0.0f,                    0.0f,                               1.0f
		);

		return Transpose(ortho);
	}

	/* 
	 * Return a Perspective Projection Matrix
	 * 
	 * References:
	 *   * http://www.songho.ca/opengl/gl_transform.html
	 *   * https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/
	 *   * http://www.songho.ca/opengl/gl_projectionmatrix.html
	 *   * https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
	 *   * http://www.opengl-tutorial.org/es/beginners-tutorials/tutorial-3-matrices/
	 *   * http://www.songho.ca/opengl/gl_lookattoaxes.html
     *   * http://www.songho.ca/opengl/gl_anglestoaxes.html
     *   * http://www.songho.ca/opengl/gl_matrix.html
	 *   * http://www.opengl-tutorial.org/es/beginners-tutorials/tutorial-3-matrices/
	 *   * https://learnopengl.com/Getting-started/Coordinate-Systems
	 */ 
	inline static Matrix4 PerspectiveProjectionMatrix(
		const float &angleOfView,  // Angle of view in degrees
		const float &aspectRatio,  // Aspect ratio (width/height of the framebuffer, not the window)
		const float &near_plane,   // Near plane
		const float &far_plane)    // Far plane
	{
		float top    = tanf(angleOfView * 0.5f * PI_OVER_180) * near_plane;
		float bottom = -top;
		float right  = top * aspectRatio;
		float left   = -right;

		// Remember to transpose
		Matrix4 proj(
			(2.0f * near_plane) / (right - left),  0.0f,                                (right + left) / (right - left),                        0.0f,
			0.0f,                                 (2.0f * near_plane) / (top - bottom), (top + bottom) / (top - bottom),                        0.0f,
			0.0f,                                  0.0f,                                -((far_plane + near_plane) / (far_plane - near_plane)), -((2.0f * far_plane * near_plane) / (far_plane - near_plane)),
			0.0f,                                  0.0f,                                -1.0f,                                                  0.0f
		);

		return Transpose(proj);
	}

	/*
	 * Return a View Matrix given the Camera vectors
	 */
	inline static Matrix4 ViewMatrix(CVector3 camEyePos, CVector3 camLookAt, CVector3 camUpVec)
	{
		CVector3 worldUp(0.0f, 1.0f, 0.0f);

		// Calculate forward vector from target to eye
		CVector3 forward = camEyePos - camLookAt;
		forward.normalize();

		// Calculate left vector
		CVector3 left = worldUp.cross(forward);
		//CVector3 left = camUpVec.cross(forward);
		left.normalize();

		// Recalculate the orthonormal up vector
		CVector3 up = forward.cross(left);
		// No need to normalize because the cross product of 2 unit vectors is another unit vector

		// Calculate inverse translation from camEyePos to camLookAt
		float inverseTranslateX = -(left.X    * camEyePos.X) - (left.Y    * camEyePos.Y) - (left.Z    * camEyePos.Z);
		float inverseTranslateY = -(up.X      * camEyePos.X) - (up.Y      * camEyePos.Y) - (up.Z      * camEyePos.Z);
		float inverseTranslateZ = -(forward.X * camEyePos.X) - (forward.Y * camEyePos.Y) - (forward.Z * camEyePos.Z);

		Matrix4 view(left.X,    left.Y,    left.Z,    inverseTranslateX,
		             up.X,      up.Y,      up.Z,      inverseTranslateY,
			         forward.X, forward.Y, forward.Z, inverseTranslateZ,
			         0.0f,      0.0f,      0.0f,      1.0f);

		return Transpose(view);
	}

	/*
	 * ==============================================================================================================
	 *                              Simple Model, View, and Projection matrices
	 * ==============================================================================================================
	 */

	 /*
	  * Simple model matrix with a rotation around Y axis
	  */
	inline static Matrix4 SimpleModelMatrixRotationY(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		return Matrix4(cosine, 0.0f, -sine, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sine, 0.0f, cosine, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	 /*
	  * A simple model matrix with a rotation around Y and a translation
	  */
	inline static Matrix4 SimpleModelMatrixRotationTranslation(float angleInRadians, CVector3 translation)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: This is already transposed
		return Matrix4(cosine, 0.0f, -sine, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sine, 0.0f, cosine, 0.0f,
			translation.getX(), translation.getY(), translation.getZ(), 1.0f);
	}

	/*
	 * 
	 */
	inline static Matrix4 SimpleViewMatrix(float cameraZDistance)
	{
		// Camera is at 60 degrees to the ground, in the YZ plane.
		// Camera Look-At is hardcoded to (0, 0, 0).
		// Camera Up is hardcoded to (0, 1, 0).
		const float sqrt3over2 = 0.86603f;

		Matrix4 vm(1.0f,  0.0f,      0.0f,            0.0f,
		           0.0f, sqrt3over2, 0.5f,            0.0f,
		           0.0f, -0.5f,      sqrt3over2,      0.0f,
		           0.0f,  0.0f,     -cameraZDistance, 1.0f);

		// ALREADY TRANSPOSED
		return vm;
	}

	/*
	 *
	 */
	inline static Matrix4 SimpleProjectionMatrix(float aspectRatio)
	{
		// Far plane is at 50.0f, near plane is at 1.0f.
		// FoV is hardcoded to pi/3.
		const float cotangent = 1 / tanf(3.14159f / 6.0f);

		Matrix4 proj(cotangent / aspectRatio, 0.0f,      0.0f,                   0.0f,
			         0.0f,                    cotangent, 0.0f,                   0.0f,
			         0.0f,                    0.0f,     -50.0f / (50.0f - 1.0f), (-50.0f * 1.0f) / (50.0f - 1.0f),
			         0.0f,                    0.0f,     -1.0f,                   0.0f);

		return Transpose(proj);
	}

	/*
	 *
	 */
	inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
	{
		os << std::fixed << std::setprecision(4);
		os << "[" << std::setw(10) << m.m[0][0] << " " << std::setw(10) << m.m[0][1] << " " << std::setw(10) << m.m[0][2] << " " << std::setw(10) << m.m[0][3] << "]\n"
		   << "[" << std::setw(10) << m.m[1][0] << " " << std::setw(10) << m.m[1][1] << " " << std::setw(10) << m.m[1][2] << " " << std::setw(10) << m.m[1][3] << "]\n"
		   << "[" << std::setw(10) << m.m[2][0] << " " << std::setw(10) << m.m[2][1] << " " << std::setw(10) << m.m[2][2] << " " << std::setw(10) << m.m[2][3] << "]\n"
		   << "[" << std::setw(10) << m.m[3][0] << " " << std::setw(10) << m.m[3][1] << " " << std::setw(10) << m.m[3][2] << " " << std::setw(10) << m.m[3][3] << "]\n";
		os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		return os;
	}
}

/*
// compute screen coordinates first
void gluPerspective(
const float &angleOfView,
const float &imageAspectRatio,
const float &n, const float &f,
float &b, float &t, float &l, float &r)
{
float scale = tan(angleOfView * 0.5 * M_PI / 180) * n;
r = imageAspectRatio * scale, l = -r;
t = scale, b = -t;
}

// set the OpenGL perspective projection matrix
void glFrustum(
const float &b, const float &t, const float &l, const float &r,
const float &n, const float &f,
Matrix44f &M)
{
// set OpenGL perspective projection matrix
M[0][0] = 2 * n / (r - l);
M[0][1] = 0;
M[0][2] = 0;
M[0][3] = 0;

M[1][0] = 0;
M[1][1] = 2 * n / (t - b);
M[1][2] = 0;
M[1][3] = 0;

M[2][0] = (r + l) / (r - l);
M[2][1] = (t + b) / (t - b);
M[2][2] = -(f + n) / (f - n);
M[2][3] = -1;

M[3][0] = 0;
M[3][1] = 0;
M[3][2] = -2 * f * n / (f - n);
M[3][3] = 0;
}

void multPointMatrix(const Vec3f &in, Vec3f &out, const Matrix44f &M)
{
//out = in * Mproj;
out.x   = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] +  in.z = 1  M[3][0];
out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] +  in.z = 1  M[3][1];
out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] +  in.z = 1  M[3][2];
float w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] +  in.z = 1  M[3][3];

// normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
if (w != 1) {
	out.x /= w;
	out.y /= w;
	out.z /= w;
}
}
*/