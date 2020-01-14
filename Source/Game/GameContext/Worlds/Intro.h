#pragma once
#include "GameWorld.h"

namespace ME
{
class IntroWorld : public GameWorld
{
private:
	HObj m_hSubtitle = INVALID_HANDLE;
	HObj m_hLeftText = INVALID_HANDLE;
	HObj m_hRightText = INVALID_HANDLE;
	Time m_elapsed;
	bool m_bCreditsShown = false;

protected:
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
