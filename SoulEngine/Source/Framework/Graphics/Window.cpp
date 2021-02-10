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
void WindowErrorCallback(Int32 iErrorCode, const char* zDescription);

namespace Soul
{
	WindowPtr Window::_poWindow = nullptr;
	WindowMode Window::_eWindowMode = WindowMode::Windowed;
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
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_zTitle = zTitle;
		_iXResolution = iXResolution;
		_iYResolution = iYResolution;

		_poWindow = glfwCreateWindow(_iXResolution, _iYResolution, _zTitle, NULL, NULL);
		if (_poWindow == NULL)
		{
			SoulLogError("Failed to create GLFW window.");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(_poWindow);
		glfwSetFramebufferSizeCallback(_poWindow, WindowResizeCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SoulLogError("Failed to load GLAD OpenGL functions.");
			return false;
		}

		glViewport(0, 0, iXResolution, iYResolution);

		_bIsSetup = true;

		return true;
	}

	void Window::ShutDown()
	{
		Assert(_bIsSetup);

		_bIsSetup = false;

		glfwTerminate();
	}

	void Window::SetWindowMode(WindowMode eMode)
	{
		Assert(_bIsSetup);

		if (_eWindowMode != eMode)
		{
			_eWindowMode = eMode;

			glfwDestroyWindow(_poWindow);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			switch (_eWindowMode)
			{
				case WindowMode::Fullscreen:
				{
					_poWindow = glfwCreateWindow(_iXResolution, _iYResolution,
						"My Title", glfwGetPrimaryMonitor(), NULL);
				} break;

				case WindowMode::Borderless:
				{
					const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

					glfwWindowHint(GLFW_RED_BITS, mode->redBits);
					glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
					glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
					glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

					_poWindow = glfwCreateWindow(mode->width, mode->height,
						"My Title", glfwGetPrimaryMonitor(), NULL);
				} break;

				case WindowMode::Windowed:
				{
					_poWindow = glfwCreateWindow(_iXResolution, _iYResolution,
						_zTitle, NULL, NULL);
					glfwSetFramebufferSizeCallback(_poWindow, WindowResizeCallback);
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

	void Window::Draw()
	{
	}

	void Window::FinalizeRender()
	{
		Assert(_bIsSetup);

		glfwSwapBuffers(_poWindow);
		glfwPollEvents();
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