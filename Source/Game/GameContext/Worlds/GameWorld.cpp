#include "Engine/GameServices.h"
#include "GameWorld.h"

namespace ME
{
GameWorld* GameWorld::s_pActive = nullptr;

GameWorld& GameWorld::Active()
{
	Assert(s_pActive, "Active GameWorld is null!");
	return *s_pActive;
}

GameWorld::GameWorld() = default;
GameWorld::GameWorld(GameWorld&&) = default;
GameWorld& GameWorld::operator=(GameWorld&&) = default;
GameWorld::~GameWorld() = default;

bool GameWorld::DestroyObject(HObj& outHandle)
{
	if (outHandle > INVALID_HANDLE && m_objectFactory.Destroy(outHandle))
	{
		outHandle = INVALID_HANDLE;
		return true;
	}
	return false;
}

void GameWorld::DestroyAll(std::vector<HObj>& outHandles)
{
	for (auto& hObj : outHandles)
	{
		DestroyObject(hObj);
	}
}

void GameWorld::DestroyAll(std::initializer_list<HObj*> outHandles)
{
	for (auto pObj : outHandles)
	{
		Assert(pObj, "Invalid pointer!");
		DestroyObject(*pObj);
	}
}

void GameWorld::OnCreated() {}
void GameWorld::OnStarting() {}

void GameWorld::Tick(Time dt)
{
	Core::RemoveIf<HObj, std::unique_ptr<GameObject>>(m_objectFactory.m_instanced, [](const auto& uObj) { return uObj->IsDestroyed(); });
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
	s_pActive = this;
	OnStarting();
}

void GameWorld::Stop()
{
	m_inputTokens.clear();
	OnStopping();
	m_objectFactory.Clear();
	s_pActive = nullptr;
	LOG_I("[%s] %s Stopped", m_name.data(), m_type.data());
}
} // namespace ME
