#pragma once
#include "../GameWorld.h"

namespace ME
{
class Tutorial5 : public GameWorld
{
protected:
	// Mouse chaser "rocket" ship
	HRes m_hShipTex = INVALID_HANDLE;
	HObj m_hRocket = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;

protected:
	void OnCreated() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
