#include <bitset>
#include <thread>
#include "SFML/Graphics.hpp"
#include "Engine/GameServices.h"
#include "Engine/Viewport/Viewport.h"
#include "GameLoop.h"

namespace ME
{
namespace
{
constexpr u8 FPS_TARGET = 60;

struct State
{
	enum Flag
	{
		INIT = 0,
		CLOSED,
		QUITTING
	};
	static const Time tickRate;
	std::bitset<3> flags;

	bool Expired() const
	{
		return flags[CLOSED] || flags[QUITTING];
	}
};

const Time State::tickRate = Time::Seconds(1.0f / FPS_TARGET);
std::unique_ptr<GameServices> uGS;
State state;
std::string workingDir;

void Init(s32 argc, char** argv)
{
#if ENABLED(DEBUG_LOGGING)
	LE::g_MinLogSeverity = LE::LogSeverity::Debug;
#endif
#if ENABLED(ASSERTS)
	std::string msg;
	msg.reserve(16);
	msg += "Invalid argc! ";
	msg += std::to_string(argc);
	AssertVar(argc > 0, msg.c_str());
#endif
	if (argc > 0)
	{
		std::string_view exePath = argv[0];
#ifdef TARGET_WIN64
		const char delim = '\\';
#else
		const char delim = '/';
#endif
		workingDir = exePath.substr(0, exePath.find_last_of(delim));
		LOG_I("Working dir: %s", workingDir.data());
	}
	uGS = std::make_unique<GameServices>();
	g_pResources->s_resourcesPath = workingDir;
	g_pResources->s_resourcesPath += "/";
	g_pResources->s_resourcesPath += "Resources";
	if (g_pResources->Init({"Default-Mono.ttf", "Default-Serif.ttf"}))
	{
		state.flags[State::INIT] = true;
	}
}

void Create(Viewport& outVP, u32 width, u32 height, const std::string& title)
{
	ViewportData data;
	data.viewportSize.width = width;
	data.viewportSize.height = height;
	data.title = title;
	outVP.SetData(std::move(data));
	outVP.Create();
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(static_cast<f32>(width), static_cast<f32>(height)));
	outVP.setView(view);
}

void PollEvents(Viewport& vp)
{
	auto vpEvent = g_pInput->PollEvents(vp);
	switch (vpEvent)
	{
	default:
		break;

	case ViewportEventType::Closed:
		state.flags[State::CLOSED] = true;
		break;
	}
}

bool Tick(Time dt)
{
	g_pInput->TakeSnapshot();
	g_pInput->FireCallbacks();

	return uGS->Tick(dt);
}

void Sleep(Time frameTime)
{
	Time residue = state.tickRate - frameTime;
	if (residue > Time::Zero)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(residue.AsMilliseconds()));
	}
}

void Cleanup()
{
	uGS->Stop();
	g_pRenderer->Clear();
	g_pResources->Clear();
	uGS = nullptr;
}
} // namespace

s32 GameLoop::Run(s32 argc, char** argv)
{
	if (!state.flags[State::INIT])
	{
		Init(argc, argv);
	}
	if (!state.flags[State::INIT])
	{
		LOG_E("[GameLoop] Fatal error initialising engine...");
		return 1;
	}
	state = State();
	Viewport viewport;
	ViewportSize size = g_pGFX->GetViewportSize();
	Create(viewport, size.width, size.height, "Untitled Game");
	uGS->Start();
	Time frameStart = Time::Now();
	Time frameTime;
	while (viewport.isOpen() && !state.Expired())
	{
		Time dt = Time::Now() - frameStart;
		frameStart = Time::Now();
		PollEvents(viewport);
		if (Tick(dt))
		{
			g_pRenderer->Render(viewport);
		}
		frameTime = Time::Now() - frameStart;
		Sleep(frameTime);
	}
	Cleanup();
	return 0;
}

void GameLoop::Stop()
{
	state.flags[State::QUITTING] = true;
}

const std::string_view GameLoop::PWD()
{
	return workingDir;
}
} // namespace ME
