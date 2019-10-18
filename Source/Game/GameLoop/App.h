#include <bitset>
#include "GameTypes.h"
#include "Engine/GameServices.h"
#include "Engine/Viewport/Viewport.h"
#pragma once

namespace ME
{
class App final : NoCopy
{
public:
	struct State
	{
		enum Flag
		{
			INIT = 0,
			CLOSED,
			QUITTING
		};
		std::bitset<3> flags;
	};

public:
	static constexpr u8 s_AA = 8;

private:
	GameServices m_services;
	Viewport m_viewport;
	std::string m_workingDir;
	State m_state;
	const Time m_tickRate;

public:
	App(u8 fpsTarget, s32 argc, char** argv);
	~App();

public:
	void CreateViewport(u32 width, u32 height, const std::string& title);
	void StartGame();

	void StartFrame();
	void PollEvents();
	void Tick(Time dt);
	void Render();
	void Sleep(Time frameTime);
	void Quit();

	bool IsInit() const;
	bool IsRunning() const;
	bool IsViewportOpen() const;
	std::string_view WorkingDir() const;
};
} // namespace ME