#pragma once
#include "../GameWorld.h"

namespace ME
{
class Temp : public GameWorld
{
private:
	std::vector<Token> m_tokens;
	// A new handle type, for a Resource (Asset)
	HRes m_hSerifFont = INVALID_HANDLE;
	HObj m_hObj0 = INVALID_HANDLE;
	HObj m_hObj1 = INVALID_HANDLE;
	HObj m_hObj2 = INVALID_HANDLE;
	// Time! It's a very simple struct that basically stores microseconds uint64_t; very useful for time _spans_, though
	Time m_obj1LayerTTL;
	bool m_bObj1LayerChanged = false;
	Time m_obj1TTL;
	bool m_bObj1Destroyed = false;

protected:
	// New callback: this is called ONLY ONCE per World, after it has been constructed
	// Use it for loading and storing resources, as they only get destroyed at the end
	// of the application, not between World loads.
	void OnCreate() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
