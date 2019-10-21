#pragma once
#include "../GameWorld.h"

namespace ME
{
class TutorialKing : public GameWorld
{
private:
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayer = INVALID_HANDLE;

protected:
	void OnCreated() override;
	void OnStarting() override;
	void OnStopping() override; // have to
};
} // namespace ME
