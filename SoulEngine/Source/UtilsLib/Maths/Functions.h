/*
A library of common math functions.
@file Functions.h
@author Jacob Peterson
@edited 4/25/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	/*
	Returns the square root of the provided float.

	@param f - Float to take the square root of. Must be positive.

	@return Float containing the resulting square root.
	*/
	Float32 SquareRoot(Float32 f);
}