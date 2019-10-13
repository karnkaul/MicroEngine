#pragma once
#include "../GameWorld.h"

namespace ME
{
class Tutorial2 : public GameWorld
{
private:
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hRectangle = INVALID_HANDLE;
	const s32 m_RECT_TTL_MIN = 2000;
	const s32 m_RECT_TTL_MAX_S = 5000;
	// Time stores spans of time and can also provide the current time
	Time m_rectTTL;
	// Just to display on screen
	Time m_rectElapsed;

protected:
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;

private:
	// A custom function to construct m_hRectangle
	void CreateRect();
};
}
