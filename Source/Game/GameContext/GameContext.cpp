#include "GameContext.h"

namespace ME
{
GameContext::GameContext() = default;
GameContext::GameContext(GameContext&&) = default;
GameContext::~GameContext() = default;

bool GameContext::LoadWorld(const std::string& id)
{
	if (m_worlds.empty())
	{
		LOG_E("[GameContext] No Gameworlds constructed!");
		return false;
	}
	auto search = m_worlds.find(id);
	if (search != m_worlds.end())
	{
		m_nextWorldID = id;
		return true;
	}
	return false;
}

bool GameContext::StartWorld(const std::string& id)
{
	if (m_worlds.empty())
	{
		LOG_E("[GameContext] No Gameworlds constructed!");
		return false;
	}
	auto search = m_worlds.find(id);
	if (search != m_worlds.end())
	{
		m_nextWorldID = id;
	}
	else
	{
		m_nextWorldID = m_worlds.begin()->second->m_name;
		LOG_W("[GameContext] [%s] Gameworld not found! Starting random GameWorld... [%s]", m_nextWorldID.data());
	}
	return true;
}

bool GameContext::Tick(Time dt)
{
	bool bDoFrame = true;
	if (!m_nextWorldID.empty())
	{
		auto search = m_worlds.find(m_nextWorldID);
		if (search != m_worlds.end())
		{
			if (m_pActive)
			{
				m_pActive->Stop();
			}
			m_pActive = search->second.get();
			m_pActive->Start();
			bDoFrame = false;
		}
		m_nextWorldID.clear();
	}
	if (m_pActive && bDoFrame)
	{
		m_pActive->Tick(dt);
	}
	return bDoFrame;
}

void GameContext::Stop()
{
	if (m_pActive)
	{
		m_pActive->Stop();
		m_pActive = nullptr;
	}
}
} // namespace ME
