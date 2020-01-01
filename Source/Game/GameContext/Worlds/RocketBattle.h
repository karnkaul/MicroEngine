#pragma once
#include "GameWorld.h"

namespace ME
{
class RocketBattleWorld : public GameWorld
{
protected:
	struct Timers
	{
		Time bubbleSpawn = Time::Seconds(5);
		Time bubbleTTL = Time::Seconds(60);
	};
	static const Timers s_defaultTimer;

	enum class GameState
	{
		Playing = 0,
		RocketDestroyed
	};

	std::vector<HObj> m_uiButtons;
	std::vector<Token> m_miscTokens;
	HPool m_hBubbles = INVALID_HANDLE;
	HPool m_hProjectiles = INVALID_HANDLE;
	HObj m_hRocket = INVALID_HANDLE;
	HObj m_hTilemap = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayerStatistics = INVALID_HANDLE;
	HObj m_hColliderBtn = INVALID_HANDLE;
	HObj m_hRestartBtn = INVALID_HANDLE;
	HRes m_hTile = INVALID_HANDLE;

	u32 m_bubbleCount = 0;
	u32 m_bubblesToSpawn = 0;
	u32 m_projectileCount = 0;
	u32 m_projectileHitCount = 0;
	GameState m_gameState = GameState::Playing;
	s32 m_playerScore = 0;
	f64 m_accuracy = 0;
	Time m_playedTime = Time::Zero;
	Timers m_delta;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
	void OnRocketDestruction();

private:
	void SetPlayerStats();
};
}
