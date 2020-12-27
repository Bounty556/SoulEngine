/*
A window that can be rendered to.
@file Window.h
@author Jacob Peterson
@edited 12/27/2020
*/

#pragma once

#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	enum class WindowModes
	{
		Fullscreen,
		Windowed,
		BorderlessWindowed
	};

	class Window
	{
	public:
		Window(UInt16 uiWindowWidth, UInt16 uiWindowHeight,
			WindowModes eWindowMode);
	};
}