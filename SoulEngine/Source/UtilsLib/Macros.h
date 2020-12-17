/*
Contains commonly used macros.
@file Macros.h
@author Jacob Peterson
@version 1 12/17/20
*/

#pragma once

#define Kilobytes(x) (x * 1024LL)
#define Megabytes(x) (Kilobytes(x) * 1024LL)
#define Gigabytes(x) (Megabytes(x) * 1024LL)

#define Assert(x) \
if (x) \
	{} \
else {*((int*)0) = 0;}