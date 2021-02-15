/*
Singleton class that manages the Engine's main window.
@file Window.cpp
@author Jacob Peterson
@edited 2/15/2021
*/

#include "Window.h"

#include <UtilsLib/Logger.h>
#include <UtilsLib/Macros.h>

void WindowResizeCallback(WindowPtr poWindow, Int32 iWidth, Int32 iHeight);
void WindowErrorCallback(Int32 iErrorCode, const char* zDescription);

namespace Soul
{
	WindowPtr Window::_poWindow = NULL;
	WindowMode Window::_eWindowMode = WindowMode::None;
	bool Window::_bIsResizable = true;
	bool Window::_bIsSetup = false;
	const char* Window::_zTitle;
	Int32 Window::_iXResolution;
	Int32 Window::_iYResolution;

	bool Window::StartUp(Int32 iXResolution, Int32 iYResolution, const char* zTitle, WindowMode eMode)
	{
		Assert(!_bIsSetup);

		if (!glfwInit())
		{
			SoulLogError("Failed to initialize GLFW.");
			return false;
		}

		glfwSetErrorCallback(WindowErrorCallback);
		
		_zTitle = zTitle;
		_iXResolution = iXResolution;
		_iYResolution = iYResolution;
		_eWindowMode = eMode;

		InitializeNewWindow();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SoulLogError("Failed to load GLAD OpenGL functions.");
			return false;
		}

		glViewport(0, 0, _iXResolution, _iYResolution);

		_bIsSetup = true;

		return true;
	}

	void Window::ShutDown()
	{
		Assert(_bIsSetup);

		_bIsSetup = false;

		glfwTerminate();
	}

	void Window::SetWindowOptions(Int32 iXResolution, Int32 iYResolution, const char* zTitle,
		WindowMode eMode)
	{
		Assert(_bIsSetup);

		_iXResolution = iXResolution;
		_iYResolution = iYResolution;
		_zTitle = zTitle;
		_eWindowMode = eMode;

		InitializeNewWindow();

		glViewport(0, 0, _iXResolution, _iYResolution);
	}

	bool Window::ShouldWindowClose()
	{
		Assert(_bIsSetup);

		return glfwWindowShouldClose(_poWindow);
	}

	void Window::PrepareForRendering()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::FinalizeRender()
	{
		Assert(_bIsSetup);

		glfwSwapBuffers(_poWindow);
		glfwPollEvents();
	}

	const char* Window::GetTitle()
	{
		return _zTitle;
	}

	WindowMode Window::GetWindowMode()
	{
		return _eWindowMode;
	}

	Int32 Window::GetXResolution()
	{
		return _iXResolution;
	}

	Int32 Window::GetYResolution()
	{
		return _iYResolution;
	}

	void Window::InitializeNewWindow()
	{
		if (_poWindow)
		{
			glfwDestroyWindow(_poWindow);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		switch (_eWindowMode)
		{
		case WindowMode::Fullscreen:
		{
			_poWindow = glfwCreateWindow(_iXResolution, _iYResolution,
				_zTitle, glfwGetPrimaryMonitor(), NULL);
		} break;

		case WindowMode::Borderless:
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			_poWindow = glfwCreateWindow(mode->width, mode->height,
				_zTitle, glfwGetPrimaryMonitor(), NULL);
		} break;

		case WindowMode::Windowed:
		{
			_poWindow = glfwCreateWindow(_iXResolution, _iYResolution,
				_zTitle, NULL, NULL);
			glfwSetFramebufferSizeCallback(_poWindow, WindowResizeCallback);
		} break;

		case WindowMode::None:
		{
			return;
		} break;
		}

		if (_poWindow == NULL)
		{
			SoulLogError("Failed to create GLFW window.");
			glfwTerminate();
		}

		glfwMakeContextCurrent(_poWindow);
	}
}

void WindowResizeCallback(WindowPtr poWindow, Int32 iWidth, Int32 iHeight)
{
	glViewport(0, 0, iWidth, iHeight);
}

void WindowErrorCallback(Int32 iErrorCode, const char* zDescription)
{
	SoulLogError("(GLFW %d) %s", iErrorCode, zDescription);
}
