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
	Float32 SquareRoot(const Float32 x)
	{
		union
		{
			Int32 i;
			Float32 x;
		} u;
		u.x = x;
		u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

		// Two Babylonian Steps (simplified from:)
		// u.x = 0.5f * (u.x + x / u.x);
		// u.x = 0.5f * (u.x + x / u.x);
		u.x =         u.x + x / u.x;
		u.x = 0.25f * u.x + x / u.x;

		return u.x;
	}
}