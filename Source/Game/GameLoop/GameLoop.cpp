#include <memory>
#include "SFML/Graphics.hpp"
#include "BuildVersion.h"
#include "Engine/Viewport/Viewport.h"
#include "GameLoop.h"
#include "GameSettings.h"
#include "App.h"
#include "OS.h"

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
			LOG_W("[GameLoop] Frame time is too high [%.2f] (max dt: [%.2f])! Game Time will appear slowed down!",
				  frameTime.AsSeconds() * 1000, maxDT.AsSeconds() * 1000);
			strikes = 0;
		}
	}
	else
	{
		strikes = 0;
	}
}
} // namespace

s32 GameLoop::Run(s32 argc, char** argv)
{
	OS::Init(argc, argv);
	GameSettings& settings = GameSettings::Inst();
#if ENABLED(DEBUG_LOGGING)
	LE::g_MinLogSeverity = static_cast<LE::LogSeverity>(settings.m_logSeverity);
#endif
	LOG_I("MicroEngine v%s (%s)", BUILD_VERSION_STR.data(), COMMIT_HASH_STR.data());
	// Create an app object and ensure it's initialised
	const Time MAX_DT = Time::Seconds(1.0f / 30);
	uApp = std::make_unique<App>(30, settings.m_maxFPS);
	if (!uApp->IsInit())
	{
		LOG_E("[GameLoop] Fatal error initialising engine...");
		return 1;
	}
	// Create a viewport (render window)
	ViewportData data;
	data.viewportSize = g_pGFX->GetViewportSize();
	data.title = "Rocket Battle";
	uApp->CreateViewport(data);
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
} // namespace ME
