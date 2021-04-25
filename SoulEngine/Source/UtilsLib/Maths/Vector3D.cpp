/*
A 3-Dimensional Vector object.
@file Vector3D.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "Vector3D.h"

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Maths/Functions.h>

namespace Soul
{
	Vector3D::Vector3D(Float32 a, Float32 b, Float32 c) :
		x(a),
		y(b),
		z(c)
	{
	}

	Float32& Vector3D::operator[](Index i)
	{
		Assert(i >= 0 && i < 3);

		return ((&x)[i]);
	}

	const Float32& Vector3D::operator[](Index i) const
	{
		Assert(i >= 0 && i < 3);

		return ((&x)[i]);
	}

	Vector3D& Vector3D::operator*=(Float32 s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3D& Vector3D::operator/=(Float32 s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3D& Vector3D::operator+=(const Vector3D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& Vector3D::operator-=(const Vector3D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3D operator*(const Vector3D& v, Float32 s)
	{
		return Vector3D(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3D operator/(const Vector3D& v, Float32 s)
	{
		s = 1.0f / s;
		return Vector3D(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3D operator-(const Vector3D& v, Float32 s)
	{
		return Vector3D(-v.x, -v.y, -v.z);
	}

	inline Vector3D operator-(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	inline Vector3D operator+(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	inline Float32 Magnitude(const Vector3D& v)
	{
		SquareRoot(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline Vector3D Normalize(const Vector3D& v)
	{
		return v / Magnitude(v);
	}
}