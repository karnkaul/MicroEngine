#include "GameInit.h"
// Include all GameWorld headers
#include "Worlds/Tutorials/Tutorial0.h"
#include "Worlds/Tutorials/Tutorial1.h"
#include "Worlds/Tutorials/Tutorial2.h"
#include "Worlds/Tutorials/Tutorial3.h"
#include "Worlds/Tutorials/Tutorial4.h"
#include "Worlds/Tutorials/Tutorial5.h"
#include "Worlds/Tutorials/Temp.h"

namespace ME
{
void GameInit::CreateWorlds()
{
	// Construct all GameWorlds in use. While the order is irrelevant in code,
	// for readability, keep it in chronological order (of development).
	ConstructWorld<Tutorial0>("Tutorial0");
	ConstructWorld<Tutorial1>("Tutorial1");
	ConstructWorld<Tutorial2>("Tutorial2");
	ConstructWorld<Tutorial3>("Tutorial3");
	ConstructWorld<Tutorial4>("Tutorial4");
	ConstructWorld<Tutorial5>("Tutorial5");
	ConstructWorld<Temp>("Temp");
}
} // namespace ME
