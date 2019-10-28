#include "ObjectPool.h"
#include "Objects/GameObject.h"

namespace ME
{
ObjectPool::ObjectPool() = default;

ObjectPool::~ObjectPool()
{
	for (auto pObj : m_objects)
	{
		pObj->m_pPool = nullptr;
	}
	LOGIF_D(!m_name.empty(), "%s destroyed", m_name.data());
}

void ObjectPool::OnCreate(std::string name)
{
	m_name = std::move(name);
	m_name += "_Pool";
	LOG_D("%s created", m_name.data());
}

void ObjectPool::SetHandle(HPool handle)
{
	m_handle = handle;
}

void ObjectPool::SetGenerator(std::function<GameObject*()> generator)
{
	m_generator = std::move(generator);
}

void ObjectPool::PreInstantiate(std::string namePrefix, s32 count /* = 1 */, bool bDespawnAll /* = true */)
{
	count -= ToS32(Despawned());
	for (s32 i = 0; i < count; ++i)
	{
		std::string name = namePrefix;
		name += "_P_";
		name += std::to_string(i);
		NewObject(std::move(name));
	}
	if (bDespawnAll)
	{
		for (auto pObj : m_objects)
		{
			pObj->Destroy();
		}
	}
}

GameObject* ObjectPool::NewObject(std::string name)
{
	for (auto pObj : m_objects)
	{
		if (pObj->m_flags[ToIdx(GameObject::Flags::Despawned)])
		{
			pObj->Respawn(std::move(name));
			return pObj;
		}
	}
	Assert(m_generator, "Generator is null!");
	if (!m_generator)
	{
		LOG_E("[%s] Cannot instantiate new Object without a generator!", Typename<ObjectPool>());
		return nullptr;
	}
	auto pObj = m_generator();
	if (pObj)
	{
		pObj->m_pPool = this;
		pObj->m_name = std::move(name);
		m_objects.push_back(pObj);
	}
	return pObj;
}

bool ObjectPool::Destroy(GameObject& obj)
{
	auto iter = Core::Search(m_objects, &obj);
	if (iter != m_objects.end())
	{
		m_objects.erase(iter);
		obj.m_pPool = nullptr;
		obj.Destroy();
		return true;
	}
	return false;
}

void ObjectPool::DestroyAll()
{
	for (auto pObj : m_objects)
	{
		pObj->m_pPool = nullptr;
		pObj->Destroy();
	}
	m_objects.clear();
}

u32 ObjectPool::Size() const
{
	return static_cast<u32>(m_objects.size());
}

u32 ObjectPool::Spawned() const
{
	u32 total = 0;
	std::for_each(m_objects.begin(), m_objects.end(), [&total](const GameObject* pObj) {
		if (pObj->m_flags[ToIdx(GameObject::Flags::Enabled)])
		{
			++total;
		}
	});
	return total;
}

u32 ObjectPool::Despawned() const
{
	u32 total = 0;
	std::for_each(m_objects.begin(), m_objects.end(), [&total](const GameObject* pObj) {
		if (pObj->m_flags[ToIdx(GameObject::Flags::Despawned)])
		{
			++total;
		}
	});
	return total;
}
} // namespace ME
