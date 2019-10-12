#pragma once
#include <memory>
#include <unordered_map>
#include "GameTypes.h"
#include "Worlds/GameWorld.h"

namespace ME
{
class GameContext : NoCopy
{
private:
	std::unordered_map<std::string, std::unique_ptr<GameWorld>> m_worlds;

public:
	GameContext();
	~GameContext();

private:
	GameWorld* m_pActive = nullptr;
	std::string m_nextWorldID;

public:
	bool LoadWorld(const std::string& id);

private:
	template <typename T>
	void ConstructWorld(std::string id);

private:
	bool StartWorld(const std::string& id);
	void Tick(Time dt);
	void Stop();

	friend class GameServices;
};

template <typename T>
void GameContext::ConstructWorld(std::string id)
{
	static_assert(std::is_base_of<GameWorld, T>::value, "T must derive from GameWorld!");
	if (id.empty())
	{
		LOG_E("[GameContext] Cannot construct World with empty ID!");
		return;
	}
	auto search = m_worlds.find(id);
	if (search != m_worlds.end())
	{
		LOG_E("[GameContext] [%s] GameWorld already constructed!", id.data());
		return;
	}
	auto uT = std::make_unique<T>();
	uT->Create(id);
	m_worlds.emplace(id, std::move(uT));
}
} // namespace ME
