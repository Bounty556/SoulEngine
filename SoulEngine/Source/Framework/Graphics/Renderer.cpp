/*
Rendering engine, handles drawing all models, sprites, etc. to the Window.
@file Renderer.cpp
@author Jacob Peterson
@edited 2/15/2021
*/

#include "Renderer.h"

#include <glad/glad.h>

namespace Soul
{
	void Renderer::StartUp()
	{
		// Nothing for now
	}

	void Renderer::ShutDown()
	{
		// Nothing for now
	}

	void Renderer::RenderTriangle()
	{
		float fpVertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		unsigned int uiVBO;
		glGenBuffers(1, &uiVBO);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(fpVertices), fpVertices, GL_STATIC_DRAW);
	}
}