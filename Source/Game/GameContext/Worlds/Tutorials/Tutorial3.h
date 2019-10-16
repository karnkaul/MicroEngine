#pragma once
#include <vector>
#include "../GameWorld.h"

namespace ME
{
class Tutorial3 : public GameWorld
{
private:
	// A bunch of custom GameObjects!
	std::vector<HObj> m_bubbles;
	HObj m_hMainText;
	LayerID m_bubbleLayer = 0;
	const s32 MIN_TTL_SECS = 2;
	const s32 MAX_TTL_SECS = 7;

protected:
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
