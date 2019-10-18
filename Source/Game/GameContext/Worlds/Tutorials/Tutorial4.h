#pragma once
#include "../GameWorld.h"

namespace ME
{
class Tutorial4 : public GameWorld
{
protected:
	// Keyboard controller object
	HObj m_hController;
	HObj m_hMainText;

protected:
	void OnCreated() override;
	void OnStarting() override;
	void OnStopping() override;
};
} // namespace ME
