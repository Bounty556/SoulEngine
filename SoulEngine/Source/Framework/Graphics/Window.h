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

		static bool ShouldWindowClose();

		static void PrepareForRendering();

		static void Draw();

		static void FinalizeRender();

		Window() = delete;

	private:
		static WindowPtr _poWindow;
		static WindowMode _eWindowMode;
		static const char* _zTitle;
		static Int32 _iXResolution;
		static Int32 _iYResolution;
		static bool _bIsResizable;
		static bool _bIsSetup;
	};
}
