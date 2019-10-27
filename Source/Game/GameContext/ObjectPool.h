#pragma once
#include <vector>
#include "GameTypes.h"
#include "Engine/Handles.h"

namespace ME
{
class ObjectPool final
{
private:
	std::vector<class GameObject*> m_objects;
	std::function<GameObject*()> m_generator;
	HPool m_handle = INVALID_HANDLE;

public:
	ObjectPool();
	~ObjectPool();

	void SetHandle(HPool handle);

	void SetGenerator(std::function<GameObject*()> generator);
	void PreInstantiate(std::string namePrefix, u32 count = 1);
	GameObject* NewObject(std::string name);
	template <typename T>
	T* NewObject(std::string name);

	bool Destroy(GameObject& obj);
	void DestroyAll();

	u32 Size() const;
	u32 Spawned() const;
	u32 Despawned() const;
};

template <typename T>
T* ObjectPool::NewObject(std::string name)
{
	return dynamic_cast<T*>(NewObject(std::move(name)));
}
} // namespace ME
