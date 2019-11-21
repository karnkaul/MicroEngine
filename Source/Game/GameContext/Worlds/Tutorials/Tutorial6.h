#pragma once
#include <vector>
#include "../GameWorld.h"

namespace ME
{
class Tutorial6 : public GameWorld
{
protected:
	std::vector<HObj> m_uiButtons;
	std::vector<Token> m_miscTokens;
	HPool m_hBubbles = INVALID_HANDLE;
	HPool m_hProjectiles = INVALID_HANDLE;
	HObj m_hRocket = INVALID_HANDLE;
	HObj m_hTilemap = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayerStatistics = INVALID_HANDLE;
	HRes m_hTile = INVALID_HANDLE;

	s32 m_bubbleCount = 0;
	s32 m_projectileCount = 0;
	s32 m_projectileHitCount = 0;
	s32 m_playerScore = 0;
	f64 m_accuracy = 0;
	Time m_playedTime = Time::Zero;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
