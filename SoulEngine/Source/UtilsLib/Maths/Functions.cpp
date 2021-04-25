/*
A library of common math functions.
@file Functions.cpp
@author Jacob Peterson
@edited 4/25/21
*/

#include "Functions.h"

namespace Soul
{
	// Definition from https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
	Float32 SquareRoot(Float32 f)
	{
		union
		{
			int i;
			float x;
		} u;
		u.x = f;
		u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

		// Two Babylonian steps (simplified from:)
		// u.x = 0.5f * (u.x + f / u.x)
		// u.x = 0.5f * (u.x + f / u.x)
		u.x =         u.x + f / u.x;
		u.x = 0.25f * u.x + f / u.x;
	}
}