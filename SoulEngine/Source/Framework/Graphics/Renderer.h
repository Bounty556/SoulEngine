/*
Rendering engine, handles drawing all models, sprites, etc. to the Window.
@file Renderer.h
@author Jacob Peterson
@edited 2/15/2021
*/

#pragma once

namespace Soul
{
	class Renderer
	{
	public:
		static void StartUp();

		static void ShutDown();
		
		static void RenderTriangle();
	};
}