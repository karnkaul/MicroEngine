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
	T* FindObject(HObj handle);
	// Destroy the `GameObject` and set the handle to INVALID
	bool DestroyObject(HObj& outHandle);
	// Destroy all valid `GameObject`s and set those handles to INVALID
	void DestroyAll(std::vector<HObj>& outHandles);
	void DestroyAll(std::initializer_list<HObj*> outHandles);

protected:
	// Called when world is created
	virtual void OnCreated();
	// Called when World is activated, must be overidden
	virtual void OnStarting() = 0;
	// Called every frame with deltaTime
	virtual void Tick(Time dt);
	// Called when World is deactivated, must be overridden
	virtual void OnStopping() = 0;

private:
	void Create(std::string id);
	void Start();
	void Stop();

	friend class GameContext;
};

// Create new `GameObject`s (at world `Start()` or other objects' `OnCreate()`)
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

// Obtain a pointer to a `GameObject` via a handle
template <typename T>
T* GameWorld::FindObject(HObj handle)
{
	return m_objectFactory.template Find<T>(handle);
}
} // namespace ME
