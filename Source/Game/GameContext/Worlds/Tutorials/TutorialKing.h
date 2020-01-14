#pragma once
#include "../GameWorld.h"

namespace ME
{
class TutorialKing : public GameWorld
{
public:
	enum GameState
	{
		PLAYING = 0,
		WINNING
	};

private:
	std::vector<Token> m_callbackTokens;
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayer = INVALID_HANDLE;

	HPool m_hFood = INVALID_HANDLE;
	s32 m_foodCount = 0;
	s32 m_gameState = GameState::PLAYING;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;

	void OnWin();
};
} // namespace ME
