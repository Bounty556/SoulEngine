/*
A library of common math functions.
@file Functions.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "Functions.h"

#define SQRT_MAGIC_F 0x5f3759df

namespace Soul
{
	// Definition from http://ilab.usc.edu/wiki/index.php/Fast_Square_Root
	// AKA The Magic Number (Quake 3)

	Float32 SquareRoot(Float32 f)
	{
		const float fhalf = 0.5f * f;

		union // Get bits for floating value
		{
			float x;
			int i;
		} u;
		u.x = f;
		u.i = SQRT_MAGIC_F - (u.i >> 1); // Gives initial guess y0
		return f * u.x * (1.5f - fhalf * u.x * u.x); // Newton step, repeating increases accuracy
	}
}