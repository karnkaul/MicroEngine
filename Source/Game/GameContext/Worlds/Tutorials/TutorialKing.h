#pragma once
#include "../GameWorld.h"

namespace ME
{
class TutorialKing : public GameWorld
{
private:
	std::vector<Token> m_callbackTokens;
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayer = INVALID_HANDLE;
	
	HPool m_hFood = INVALID_HANDLE;
	s32 m_foodCount = 0;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override; 
};
} // namespace ME
