#pragma once
#include <string>
#include "GameTypes.h"
#include "Engine/Handles.h"

namespace ME
{
class GameWorld
{
protected:
	std::vector<Token> m_tokens;
	std::string m_id;

public:
	GameWorld();
	GameWorld(GameWorld&&);
	virtual ~GameWorld();

public:
	std::string_view ID() const;

protected:
	virtual void OnCreated();
	// Called when World is activated
	virtual void Start() = 0;
	// Called when World is deactivated
	virtual void Stop();
	// Called every frame with deltaTime
	virtual void Tick(Time dt);

private:
	void Create(std::string id);

	friend class GameContext;
};
} // namespace ME
