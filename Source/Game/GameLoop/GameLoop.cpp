#include "SFML/Graphics.hpp"
#include "Engine/Viewport/Viewport.h"
#include "GameLoop.h"
#include "App.h"

namespace ME
{
namespace
{
constexpr u8 FPS_TARGET = 60;
std::unique_ptr<App> uApp;
} // namespace

s32 GameLoop::Run(s32 argc, char** argv)
{
	// Create an app object and ensure it's initialised
	uApp = std::make_unique<App>(FPS_TARGET, argc, argv);
	if (!uApp->IsInit())
	{
		LOG_E("[GameLoop] Fatal error initialising engine...");
		return 1;
	}
	// Create a viewport (render window)
	ViewportSize size = g_pGFX->GetViewportSize();
	uApp->CreateViewport(size.width, size.height, "Untitled Game");
	// Construct Worlds and start the game
	uApp->StartGame();

	// The core Game Loop
	Time frameStart = Time::Now();
	Time frameTime;
	while (uApp->IsRunning())
	{
		Time dt = Time::Now() - frameStart;
		frameStart = Time::Now();
		// Build and render frame
		uApp->StartFrame();
		uApp->PollEvents();
		uApp->Tick(dt);
		uApp->Render();
		// If frame completed before 1/60 ms, sleep this thread for the remaining time
		frameTime = Time::Now() - frameStart;
		uApp->Sleep(frameTime);
	}
	// Destroy the app and cleanup its resources
	uApp = nullptr;
	// All went well
	return 0;
}

void GameLoop::Stop()
{
	// Set the CLOSED flag
	uApp->Quit();
}

const std::string_view GameLoop::PWD()
{
	// Might be useful utility, eg, to save/load files relative to executable path
	return uApp->WorkingDir();
}
} // namespace ME
