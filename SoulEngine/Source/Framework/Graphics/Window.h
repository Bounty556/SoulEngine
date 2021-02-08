/*
Singleton class that manages the Engine's main window.
@file Window.h
@author Jacob Peterson
@edited 2/8/2021
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UtilsLib/CommonTypes.h>

typedef GLFWwindow* WindowPtr;

namespace Soul
{
	enum class WindowMode
	{
		Fullscreen,
		Borderless,
		Windowed
	};

	class Window
	{
	public:

		static bool StartUp(Int32 iXResolution, Int32 iYResolution, const char* zTitle,
			WindowMode eMode);

		static void ShutDown();

		static void SetWindowMode(WindowMode eMode);

		static void SetResizable(bool bIsResizable);

		static bool ShouldWindowClose();

		static void PrepWindowForRendering();

		static void Render();

		Window() = delete;

	private:
		static WindowPtr _poWindow;
		static WindowMode _eWindowMode;
		static bool _bIsResizable;
		static bool _bIsSetup;
	};
}
