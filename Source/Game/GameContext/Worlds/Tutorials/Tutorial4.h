#pragma once
#include "../GameWorld.h"

namespace ME
{
class Tutorial4 : public GameWorld
{
protected:
	// Keyboard controller object
	HObj m_hController = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;

protected:
	void OnCreate() override;
	void OnStarting() override;
	void OnStopping() override;
};
} // namespace ME
