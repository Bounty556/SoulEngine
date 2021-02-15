/*
Entry point for the Client side of this app.
@file Main.cpp
@author Jacob Peterson
*/

#include <Events/EventBus.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Macros.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>
#include <UtilsLib/Logger.h>
#include <UtilsLib/Timer.h>

#include <TestsLib/TestRunner.h>

#include <Framework/Graphics/Window.h>
#include <Framework/Graphics/Renderer.h>

void StartUp();
void ShutDown();

int main()
{
	Soul::Timer oTimer;
	oTimer.Start();

	StartUp();

	Soul::TestRunner::RunAllTestSuites();

	// Measure time taken to startup and test
	oTimer.Stop();
	SoulLogInfo("Time taken:\n\tSeconds: %llf\n\tMillis: %llf\n\tMicros: %llf",
		oTimer.GetElapsedSeconds(), oTimer.GetElapsedMilliseconds(), oTimer.GetElapsedMicroseconds());

	// Main game loop
	while (!Soul::Window::ShouldWindowClose())
	{
		// TODO: Add input manager

		Soul::Window::PrepareForRendering();

		// Draw calls
		Soul::Renderer::RenderTriangle();

		Soul::Window::FinalizeRender();
	}

	// Clean up
	ShutDown();

	return 0;
}

void StartUp()
{
	Soul::MemoryManager::StartUp(Gigabytes(1), Megabytes(8));
	Soul::EventBus::StartUp(32);
	Soul::Window::StartUp(640, 480, "SoulEngine", Soul::WindowMode::Windowed);
	Soul::Renderer::StartUp();
}

void ShutDown()
{
	Soul::Renderer::ShutDown();
	Soul::Window::ShutDown();
	Soul::EventBus::Shutdown();

	Assert(Soul::MemoryManager::GetTotalAllocatedBytes() == 0);
	Soul::MemoryManager::Shutdown();
}