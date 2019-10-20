#include <memory>
#include "SFML/Graphics.hpp"
#include "Engine/Viewport/Viewport.h"
#include "GameLoop.h"
#include "App.h"

namespace ME
{
namespace
{
std::unique_ptr<App> uApp;

void ProfileFrameTime(Time frameTime, Time maxDT)
{
	constexpr u8 MAX_STRIKES = 3;
	static u8 strikes = 0;
	if (frameTime > maxDT)
	{
		++strikes;
		if (strikes >= MAX_STRIKES)
		{
			LOG_W("[GameLoop] Frame time is too high [%.2f] (max dt: [%.2f])! Game Time will appear slowed down!", frameTime.AsSeconds() * 1000, maxDT.AsSeconds() * 1000);
			strikes = 0;
		}
	}
}
} // namespace

s32 GameLoop::Run(u8 minFPs, u8 maxFPS, s32 argc, char** argv)
{
	// Create an app object and ensure it's initialised
	const Time MAX_DT = Time::Seconds(1.0f / minFPs);
	uApp = std::make_unique<App>(minFPs, maxFPS, argc, argv);
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
		Time dt = Maths::Clamp(Time::Now() - frameStart, Time::Zero, MAX_DT);
		frameStart = Time::Now();
		// Build and render frame
		uApp->StartFrame();
		uApp->PollEvents();
		uApp->Tick(dt);
		uApp->Render();
		// If frame completed before 1/60 ms, sleep this thread for the remaining time
		frameTime = Time::Now() - frameStart;
		ProfileFrameTime(frameTime, MAX_DT);
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
