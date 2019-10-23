#pragma once
#include "GameTypes.h"
#include "Engine/GameServices.h"

namespace ME
{
class GameInit
{
private:
	static void CreateWorlds();

	template <typename T>
	static void ConstructWorld(std::string id);

	friend class GameServices;
};

template <typename T>
void GameInit::ConstructWorld(std::string id)
{
	Assert(g_pContext, "Context is null!");
	g_pContext->ConstructWorld<T>(id);
}
}
