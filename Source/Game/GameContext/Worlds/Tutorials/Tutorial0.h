#include "../GameWorld.h"

namespace ME
{
// This class demonstrates the fundamentals of a GameWorld and some basic functionality
class Tutorial0 : public GameWorld
{
private:
	// HObj is a Handle to a (Game)Object, it will "point" to an instance and can be used
	// to retrieve the instance from the owner (this GameWorld object).
	// Initialising it to INVALID is good practice for debugging.
	HObj m_hMainText = INVALID_HANDLE;

public:
	// These overrides are called by GameContext (owns all GameWorlds).
	// OnStarting() is called when this GameWorld instance is activated,
	// use it to instantiate new GameObjects etc.
	void OnStarting() override;

	// OnStopping() is called when this GameWorld instance is deactivating;
	// all instantiated GameObjects will be destroyed after this call returns.
	// Thus we should invalidate their handles here.
	void OnStopping() override;
};
} // namespace ME
