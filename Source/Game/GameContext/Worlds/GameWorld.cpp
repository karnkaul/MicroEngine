#include "Engine/GameServices.h"
#include "GameWorld.h"

namespace ME
{
GameWorld::GameWorld() = default;
GameWorld::GameWorld(GameWorld&&) = default;
GameWorld& GameWorld::operator=(GameWorld&&) = default;
GameWorld::~GameWorld() = default;

bool GameWorld::DestroyObject(HObj handle)
{
	if (handle > INVALID_HANDLE)
	{
		return m_objectFactory.Destroy(handle);
	}
	return false;
}

void GameWorld::OnCreated() {}
void GameWorld::OnStarting() {}

void GameWorld::Tick(Time dt) 
{
	for (auto& kvp : m_objectFactory.m_instanced)
	{
		auto& obj = kvp.second;
		if (obj->IsEnabled())
		{
			obj->Tick(dt);
		}
	}
}

void GameWorld::OnStopping() {}

void GameWorld::Create(std::string name)
{
	m_name = std::move(name);
	Type(); // Call updates Type to `this`
	OnCreated();
	LOG_I("[GameContext] [%s] GameWorld created", m_name.data());
}

void GameWorld::Start()
{
	LOG_I("[%s] %s Started", m_name.data(), m_type.data());
	OnStarting();
}

void GameWorld::Stop()
{
	m_inputTokens.clear();
	OnStopping();
	m_objectFactory.Clear();
	LOG_I("[%s] %s Stopped", m_name.data(), m_type.data());
}
} // namespace ME
