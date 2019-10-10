#include <bitset>
#include <thread>
#include "SFML/Graphics.hpp"
#include "Engine/Input/Input.h"
#include "Engine/Resources/Resources.h"
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
Resources resources;
State state;
std::string workingDir;

std::unique_ptr<sf::CircleShape> uCircle;

void Init(s32 argc, char** argv)
{
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
	resources.s_resourcesPath = workingDir;
	resources.s_resourcesPath += "/";
	resources.s_resourcesPath += "Resources";
	if (resources.Init({"Default-Mono.ttf", "Default-Serif.ttf"}))
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

void PollEvents(Input& input, Viewport& vp)
{
	auto vpEvent = input.PollEvents(vp);
	switch (vpEvent)
	{
	default:
		break;

	case ViewportEventType::Closed:
		state.flags[State::CLOSED] = true;
		break;
	}
}

void Tick(Input& input, Time /*dt*/)
{
	input.TakeSnapshot();
	input.FireCallbacks();
	// Fire input
	// Update game state

	if (!uCircle)
	{
		uCircle = std::make_unique<sf::CircleShape>();
		uCircle->setFillColor(sf::Color::Yellow);
		uCircle->setRadius(100.0f);
		uCircle->setOrigin({100.0f, 100.0f});
	}
}

void Render(sf::RenderWindow& rw)
{
	if (uCircle)
	{
		rw.draw(*uCircle);
	}
	rw.display();
	rw.clear();
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
	uCircle = nullptr;
	resources.Clear();
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
	Input input;
	Token t = input.Register([](const Input::Frame& frame) -> bool {
		LOGIF_I(frame.IsPressed(KeyCode::A), "A pressed!");
		return false;
	});
	Viewport viewport;
	Create(viewport, 1280, 720, "Test");
	Time frameStart = Time::Now();
	Time frameTime;
	while (viewport.isOpen() && !state.Expired())
	{
		Time dt = Time::Now() - frameStart;
		frameStart = Time::Now();
		PollEvents(input, viewport);
		Tick(input, dt);
		Render(viewport);
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
