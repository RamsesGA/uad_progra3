#include "../stdafx.h"

#include <math.h>
#include "../Include/CVector3.h"

/*
*/
CVector3::CVector3() :
	X(0.0f), 
	Y(0.0f), 
	Z(0.0f) 
{
}

/*
*/
CVector3::CVector3(float _x, float _y, float _z)
	: X(_x), Y(_y), Z(_z)
{
}

/*
*/
CVector3::CVector3(const CVector3 &other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

/*
*/
CVector3::~CVector3()
{
}

/*
*/
CVector3& CVector3::operator=(const CVector3 &other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;

	return *this;
}

/*
*/
CVector3& CVector3::operator+=(const CVector3 &other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

/*
*/
CVector3& CVector3::operator-=(const CVector3 &other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

/*
*/
CVector3 CVector3::cross(const CVector3 & other)
{
	CVector3 res;
	res.setValues(
		this->Y*other.Z - this->Z*other.Y,
		this->Z*other.X - this->X*other.Z,
		this->X*other.Y - this->Y*other.X
	);
	return res;
}

/*
*/
float CVector3::dot(const CVector3 & other)
{
	return ((this->X * other.X) + (this->Y * other.Y) + (this->Z * other.Z));
}

/*
*/
CVector3 CVector3::operator+(const CVector3 & other)
{
	CVector3 res;
	res.setValues(
		this->X + other.X,
		this->Y + other.Y,
		this->Z + other.Z
	);
	return res;
}

/*
*/
CVector3 CVector3::operator-(const CVector3 & other)
{
	CVector3 res;
	res.setValues(
		this->X - other.X,
		this->Y - other.Y,
		this->Z - other.Z
	);
	return res;
}

/*
*/
CVector3 CVector3::operator*(float delta)
{
	CVector3 res;
	res.setValues(
		this->X * delta,
		this->Y * delta,
		this->Z * delta
	);
	return res;
}

/*
*/
CVector3& CVector3::operator*=(float delta)
{
	X *= delta;
	Y *= delta;
	Z *= delta;

	return *this;
}

/*
*/
void CVector3::normalize()
{
	float mag = magnitude();
	X /= mag;
	Y /= mag;
	Z /= mag;
}

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
// https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
/*
*/
float CVector3::quickReverseSqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

/*
*/
float CVector3::quickReverseSqrt2(float number)
{
	union {
		float f;
		long i;
	} conv;

	float x2;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	conv.f = number;
	conv.i = 0x5f3759df - (conv.i >> 1);	// what the fuck? 
	conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
	return conv.f;
}

/*
*/
float CVector3::magnitude()
{
	float len = (X * X) + (Y * Y) + (Z * Z);
	float invSqRoot = quickReverseSqrt2(len);
	float sqRoot = 1 / invSqRoot;

	return sqRoot;
	//return sqrt(len);
}

/*
*/
void CVector3::setValues(float _x, float _y, float _z)
{
	X = _x;
	Y = _y;
	Z = _z;
}

/*
*/
void CVector3::setValues(float *values)
{
	X = *(values + 0);
	Y = *(values + 1);
	Z = *(values + 2);
}