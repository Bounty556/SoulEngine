/*
A 3-Dimensional Vector object.
@file Vector3D.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "Vector3D.h"

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>

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
}