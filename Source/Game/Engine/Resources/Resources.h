#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "GameTypes.h"
#include "Factory/Factory.h"
#include "Engine/Handles.h"
#include "Resource.h"

namespace ME
{
extern std::vector<Font*> g_defaultFonts;

class Resources final : NoCopy
{
public:
	static std::string s_resourcesPath;

private:
	UFactory<Resource> m_factory;
	std::unordered_map<std::string, HRes> m_idToHandle;

public:
	Resources();
	~Resources();

public:
	bool Init(std::initializer_list<std::string> defaultFonts);
	void Clear();

public:
	std::string ResourcePath(const std::string& id) const;
	bool IsFilePresent(const std::string& path) const;
	HRes FindID(const std::string& id) const;

public:
	template <typename T>
	HRes Load(const std::string& id);

	template <typename T>
	T* Find(HRes handle) const;
};

template <typename T>
HRes Resources::Load(const std::string& id)
{
	static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource!");
	auto handle = FindID(id);
	if (handle != INVALID_HANDLE)
	{
		return handle;
	}
	std::string path = ResourcePath(id);
	if (IsFilePresent(path))
	{
		auto handle = m_factory.template New<T>();
		auto pT = m_factory.template Find<T>(handle);
		if (pT)
		{
			if (pT->Load(id, std::move(path)))
			{
				m_idToHandle.emplace(id, handle);
				LOG_I("[Resources] [%s] %s loaded", pT->m_id.data(), pT->m_type.data());
				return handle;
			}
			else
			{
				LOG_W("[Resources] [%s] Error loading %s! [%s]", pT->m_id.data(), pT->m_type.data(), pT->m_path.data());
				m_factory.Destroy(handle);
			}
		}
	}
	else
	{
		LOG_W("[Resources] [%s] %s not present on filesystem! [%s]", id.data(), Typename<T>().data(), path.data());
	}
	return INVALID_HANDLE;
}

template <typename T>
T* Resources::Find(HRes handle) const
{
	static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource!");
	return (handle > INVALID_HANDLE) ? m_factory.template Find<T>(handle) : nullptr;
}
} // namespace ME
