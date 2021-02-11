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
		Windowed,
		None // Windows should never have their mode set to None, for initialization only.
	};

	/*
	The Window singleton. The Soul Engine only support one window at a time.
	Before any window shows up or any OpenGL functions can be called, the
	Window class must first be setup via the StartUp() function. The main
	game/render loop can then be setup as follows:

	Window::StartUp(640, 480, "Window", WindowMode::Windowed);

	while (!Window::ShouldWindowClose())
	{
		// Game updates here

		Window::PrepareForRendering();

		// Rendering here

		Window::FinalizeRender();
	}
	Window::ShutDown();

	The window's windowing mode, resolution, and title can also be changed at
	any time via the SetWindowOptions(...) function.

	*/
	class Window
	{
	public:

		/*
		Initializes the Window singleton, as well as all of the necessary OpenGL
		functions.

		@param iXResolution - The window's X resolution.

		@param iYResolution - The window's Y resolution.

		@param zTitle - The window's title.

		@param eMode - The window's windowing mode.

		@return bool containing whether all of the necessary libraries were
		             correctly initialized.
		*/
		static bool StartUp(Int32 iXResolution, Int32 iYResolution, const char* zTitle,
			WindowMode eMode);

		/*
		Shuts down the Window class and all necessary OpenGL libraries.
		*/
		static void ShutDown();

		/*
		Changes the currently displayed Window's settings to whatever is
		provided in the parameters.

		@param iXResolution - The window's X resolution.

		@param iYResolution - The window's Y resolution.

		@param zTitle - The window's title.

		@param eMode - The window's windowing mode.
		*/
		static void SetWindowOptions(Int32 iXResolution, Int32 iYResolution, const char* zTitle,
			WindowMode eMode);

		/*
		Whether the window should be closed.

		@return bool containing whether the window was queried to be closed.
		*/
		static bool ShouldWindowClose();

		/*
		Sets the window up to be rendered to.
		*/
		static void PrepareForRendering();

		/*
		Temporary draw function.
		*/
		static void Draw();

		/*
		Swaps the draw buffers, rendering this frame's final image to the
		window.
		*/
		static void FinalizeRender();

		/*
		The current title of the window.

		@return const char* containing the title of the window.
		*/
		static const char* GetTitle();

		/*
		The current windowing mode of the window.

		@return WindowMode containing the current windowing mode of the window.
		*/
		static WindowMode GetWindowMode();

		/*
		The current X resolution of the window.

		@return Int32 containing the X resolution of the window.
		*/
		static Int32 GetXResolution();

		/*
		The current Y resolution of the window.

		@return Int32 containing the Y resolution of the window.
		*/
		static Int32 GetYResolution();

		Window() = delete;

	private:
		/*
		Creates a new window given the private member variables of the Window
		singleton.
		*/
		static void InitializeNewWindow();

	private:
		static WindowPtr _poWindow; // Pointer to the GLFW window object
		static WindowMode _eWindowMode; // Current windowing mode
		static const char* _zTitle; // Title of the window
		static Int32 _iXResolution; // X resolution of the window
		static Int32 _iYResolution; // Y resolution of the window
		static bool _bIsResizable; // Whether this window is resizable
		static bool _bIsSetup; // Whether the Window singleton has been setup
	};
}
