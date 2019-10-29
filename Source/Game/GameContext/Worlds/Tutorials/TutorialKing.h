#pragma once
#include "../GameWorld.h"

namespace ME
{
class TutorialKing : public GameWorld
{
private:
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPlayer = INVALID_HANDLE;
	HObj m_hEnemy = INVALID_HANDLE;

protected:
	void OnCreate() override;
	void OnStarting() override;
	//void Tick(Time dt) override;
	void OnStopping() override; 
};
} // namespace ME
