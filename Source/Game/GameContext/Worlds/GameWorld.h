#pragma once
#include <memory>
#include <type_traits>
#include "Factory/Factory.h"
#include "../GameObjectBase.h"
#include "../Objects/GameObject.h"

namespace ME
{
class GameWorld : public GameObjectBase
{
private:
	static GameWorld* s_pActive;

private:
	UFactory<GameObject> m_objectFactory;
	
public:
	static GameWorld& Active();

public:
	GameWorld();
	GameWorld(GameWorld&&);
	GameWorld& operator=(GameWorld&&);
	virtual ~GameWorld();

public:
	template <typename T>
	HObj NewObject(std::string name);
	template <typename T>
	GameObject* FindObject(HObj handle);
	bool DestroyObject(HObj& outHandle);

protected:
	virtual void OnCreated();
	// Called when World is activated
	virtual void OnStarting();
	// Called every frame with deltaTime
	virtual void Tick(Time dt);
	// Called when World is deactivated
	virtual void OnStopping();

private:
	void Create(std::string id);
	void Start();
	void Stop();

	friend class GameContext;
};

template <typename T>
HObj GameWorld::NewObject(std::string name)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject!");
	auto h = m_objectFactory.template New<T>();
	auto pT = m_objectFactory.template Find<T>(h);
	if (pT)
	{
		pT->Create(std::move(name));
	}
	return h;
}

template <typename T>
GameObject* GameWorld::FindObject(HObj handle)
{
	return m_objectFactory.template Find<T>(handle);
}
} // namespace ME
