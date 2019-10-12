#include "GameWorld.h"

namespace ME
{
GameWorld::GameWorld() = default;
GameWorld::GameWorld(GameWorld&&) = default;
GameWorld::~GameWorld() = default;

std::string_view GameWorld::ID() const
{
	return m_id;
}

void GameWorld::OnCreated() {}
void GameWorld::Start() {}
void GameWorld::Stop() {}
void GameWorld::Tick(Time /*dt*/) {}

void GameWorld::Create(std::string name)
{
	m_id = std::move(name);
	OnCreated();
	LOG_I("[GameContext] [%s] GameWorld created", m_id.data());
}
} // namespace ME
