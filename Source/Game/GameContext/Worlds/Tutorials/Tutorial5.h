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
	// Various callback registration tokens for buttons and collisions
	std::vector<Token> m_miscTokens;
	// New Handle: for ObjectPools (owned by GameWorld)
	HPool m_hBubbles = INVALID_HANDLE;
	HPool m_hProjectiles = INVALID_HANDLE;
	// Mouse chaser "rocket" ship
	HObj m_hRocket = INVALID_HANDLE;
	// Tilemap : covers screen with tiles of a given texture
	HObj m_hTilemap = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;
	// Texture for Tilemap
	HRes m_hTile = INVALID_HANDLE;
	s32 m_bubbleCount = 0;
	s32 m_projectileCount = 0;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
