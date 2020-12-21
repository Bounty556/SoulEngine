/*
Contains commonly used macros.
@file Macros.h
@author Jacob Peterson
@edited 12/21/20
*/

#pragma once

#define Kilobytes(x) (x * 1024LL)
#define Megabytes(x) (Kilobytes(x) * 1024LL)
#define Gigabytes(x) (Megabytes(x) * 1024LL)

#define ByteDistance(x, y) ((size_t)((unsigned char*)y - (unsigned char*)x))

#define Assert(x) \
if (x) \
	{} \
else {*((int*)0) = 0;}

#define ToString(x) #x
#define Stringify(x) ToString(x)