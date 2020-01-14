#include "GameInit.h"
// Include all GameWorld headers
#include "Worlds/Intro.h"
#include "Worlds/RocketBattle.h"

namespace ME
{
void GameInit::CreateWorlds()
{
	// Construct all GameWorlds in use. While the order is irrelevant in code,
	// for readability, keep it in chronological order (of development).
	ConstructWorld<IntroWorld>("Intro");
	ConstructWorld<RocketBattleWorld>("RocketBattle");
}
} // namespace ME
