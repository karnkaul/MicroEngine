#pragma once
#include <vector>
#include "../GameWorld.h"

namespace ME
{
class Tutorial5 : public GameWorld
{
protected:
	// Various UI Button handles
	std::vector<HObj> m_uiButtons;
	// New Handle: for ObjectPools (owned by GameWorld)
	HPool m_hBubbles;
	HPool m_hProjectiles;
	// Mouse chaser "rocket" ship
	HObj m_hRocket = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;
	s32 m_bubbleCount = 0;
	s32 m_projectileCount = 0;
	std::vector<Token> m_collisionTokens;

protected:
	void OnCreated() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
