#pragma once
#include <bitset>
#include "GameTypes.h"
#include "Engine/GameServices.h"
#include "Engine/Viewport/Viewport.h"

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

private:
	GameServices m_services;
	Viewport m_viewport;
	State m_state;
	const Time m_TICK_RATE;
	const Time m_MAX_DT;

#if defined(DEBUGGING)
	std::vector<Token> m_debugTokens;
#endif

public:
	App(u8 minFPS, u8 maxFPS);
	~App();

public:
	void CreateViewport(const ViewportData& data);
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
};
} // namespace ME
