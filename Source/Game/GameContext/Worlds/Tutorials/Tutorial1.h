#include "../GameWorld.h"

namespace ME
{
class Tutorial1 final : public GameWorld
{
private:
	// Models a 2D point (or a unit vector orientation)
	Vector2 m_pointerPos;
	// A new handle type, for a Resource (Asset - alive throughout the app session)
	HRes m_hSerifFont = INVALID_HANDLE;
	HObj m_hMainText = INVALID_HANDLE;
	HObj m_hPointerCircle = INVALID_HANDLE;
	// Will be toggled via input
	bool m_bPointerFollow = false;

protected:
	// New callback: this is called ONLY ONCE per World, after it has been constructed
	// Use it for loading and storing resources, as they only get destroyed at the end
	// of the application, not between World loads.
	void OnCreated() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
} // namespace ME
