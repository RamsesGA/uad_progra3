#pragma once

#ifndef CVECTOR3_H
#define CVECTOR3_H

// Simple class to practice C++ class creation
// It will be more helpful later when more code is added
class CVector3
{
public:
	float X, Y, Z;

public:
	CVector3();
	CVector3(float _x, float _y, float _z);
	CVector3(const CVector3 &other);
	~CVector3();

	CVector3& operator=(const CVector3 &other);
	
	CVector3 operator+(const CVector3 & other);
	CVector3& operator+=(const CVector3 &other);

	CVector3 operator-(const CVector3 & other);
	CVector3& operator-=(const CVector3 &other);

	CVector3 operator*(float delta);
	CVector3& operator*=(float delta);

	CVector3 cross(const CVector3 & other);
	float dot(const CVector3 & other);
	
	void setValues(float _x, float _y, float _z);
	void setValues(float *values);
	float getX() const { return X; }
	float getY() const { return Y; }
	float getZ() const { return Z; }
	void getValues(float *values) const { values[0] = X; values[1] = Y; values[2] = Z; }
	void normalize();
	float magnitude();

	inline static CVector3 ZeroVector()
	{
		return CVector3(0.0f, 0.0f, 0.0f);
	}

private:
	float quickReverseSqrt(float number);
	float quickReverseSqrt2(float number);
};

#endif // !CVECTOR3_H
