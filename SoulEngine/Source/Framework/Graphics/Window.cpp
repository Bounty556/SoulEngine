/*
Singleton class that manages the Engine's main window.
@file Window.cpp
@author Jacob Peterson
@edited 2/8/2021
*/

#include "Window.h"

#include <UtilsLib/Logger.h>
#include <UtilsLib/Macros.h>

void WindowResizeCallback(WindowPtr poWindow, Int32 iWidth, Int32 iHeight);

namespace Soul
{
	WindowPtr Window::_poWindow = nullptr;
	WindowMode Window::_eWindowMode = WindowMode::Windowed;
	bool Window::_bIsResizable = true;
	bool Window::_bIsSetup = false;

	bool Window::StartUp(Int32 iXResolution, Int32 iYResolution, const char* zTitle, WindowMode eMode)
	{
		Assert(!_bIsSetup);

		if (!glfwInit())
		{
			SoulLogError("Failed to initialize GLFW.");
			return false;
		}
		
		glfwWindowHint(GLFW_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_poWindow = glfwCreateWindow(iXResolution, iYResolution, zTitle, NULL, NULL);
		if (_poWindow == NULL)
		{
			SoulLogError("Failed to create GLFW window.");
			glfwTerminate();
			return false;
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SoulLogError("Failed to load GLAD OpenGL functions.");
			return false;
		}

		glViewport(0, 0, iXResolution, iYResolution);
		glfwSetFramebufferSizeCallback(_poWindow, WindowResizeCallback);

		_bIsSetup = true;

		return true;
	}

	void Window::ShutDown()
	{
		Assert(_bIsSetup);

		glfwTerminate();
	}

	void Window::SetWindowMode(WindowMode eMode)
	{
		Assert(_bIsSetup);

		if (_eWindowMode != eMode)
		{
			_eWindowMode = eMode;
			// TODO: Made the window mode actually effect the window
		}
	}

	void Window::SetResizable(bool bIsResizable)
	{
		Assert(_bIsSetup);

		_bIsResizable = bIsResizable;
		// TODO: Make window resizable or not
	}

	bool Window::ShouldWindowClose()
	{
		Assert(_bIsSetup);

		return glfwWindowShouldClose(_poWindow);
	}

	void Window::PrepWindowForRendering()
	{
		Assert(_bIsSetup);

		glfwSwapBuffers(_poWindow);
		glfwPollEvents();
	}

	void Window::Render()
	{

	}

}

void WindowResizeCallback(WindowPtr poWindow, Int32 iWidth, Int32 iHeight)
{
	glViewport(0, 0, iWidth, iHeight);
}