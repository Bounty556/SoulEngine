/*
A 3-Dimensional Vector object.
@file Vector3D.h
@author Jacob Peterson
@edited 4/25/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Maths/Functions.h>

namespace Soul
{
	struct Vector3D
	{
		Vector3D() = default;
		Vector3D(Float32 a, Float32 b, Float32 c);

		Float32& operator[](Index i);
		const Float32& operator[](Index i) const;
		Vector3D& operator*=(Float32 s);
		Vector3D& operator/=(Float32 s);
		Vector3D& operator+=(const Vector3D& v);
		Vector3D& operator-=(const Vector3D& v);

		Float32 x, y, z;
	};

	inline Vector3D operator*(const Vector3D& v, Float32 s)
	{
		return Vector3D(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3D operator/(const Vector3D& v, Float32 s)
	{
		s = 1.0f / s;
		return Vector3D(v.x * s, v.y * s, v.z * s);
	}

	inline Vector3D operator-(const Vector3D& v)
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

	/*
	Returns the magnitude of the provided vector as a Float32.

	@param v - Vector to calculate the magnitude of.

	@return Float32 containing the magnitude of the provided vector.
	*/
	inline Float32 Magnitude(const Vector3D& v)
	{
		return SquareRoot(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	/*
	Returns a unit vector based in the same direction as the provided vector.

	@param v - Vector to normalize.

	@return Vector3D of unit length in the direction of the provided vector.
	*/
	inline Vector3D Normalize(const Vector3D& v)
	{
		return v / Magnitude(v);
	}
}