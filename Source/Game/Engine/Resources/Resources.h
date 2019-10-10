#pragma once
#include "Factory/Factory.h"
#include "SFML/Graphics/Font.hpp"
#include "NoCopy.h"
#include "GameTypes.h"

namespace ME
{
class Resource : NoCopy
{
protected:
	std::string m_id;
	std::string m_path;
	std::string m_type = "Unknown";
	bool m_bOK = false;

public:
	Resource();
	virtual ~Resource();

public:
	bool Load(std::string id, std::string path);

protected:
	virtual bool OnLoad() = 0;

private:
	friend class Resources;
};

class Font final : public Resource
{
public:
	sf::Font m_font;

public:
	Font();
	~Font() override;

protected:
	bool OnLoad() override;
};

extern class Resources* g_pResources;

class Resources final : NoCopy
{
public:
	using HResource = s32;
	static const HResource INVALID = -1;

public:
	static std::string s_resourcesPath;

private:
	UFactory<Resource> m_factory;
	std::unordered_map<std::string, HResource> m_idToHandle;

public:
	Resources();
	~Resources();

public:
	bool Init(std::initializer_list<std::string> defaultFonts);
	void Clear();

public:
	std::string ResourcePath(const std::string& id) const;
	bool IsFilePresent(const std::string& path) const;
	HResource FindID(const std::string& id) const;

public:
	template <typename T>
	HResource Load(const std::string& id);

	template <typename T>
	T* Find(HResource handle) const;
};

template <typename T>
Resources::HResource Resources::Load(const std::string& id)
{
	static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource!");
	auto handle = FindID(id);
	if (handle != INVALID)
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
				LOG_I("[Resources] [%s] %s (Resource) loaded", pT->m_id.data(), pT->m_type.data(), pT->m_path.data());
				return handle;
			}
			else
			{
				LOG_W("[Resources] [%s] Error loading %s (Resource)! [%s]", pT->m_id.data(), pT->m_type.data(), pT->m_path.data());
				m_factory.Destroy(handle);
			}
		}
	}
	else
	{
		LOG_W("[Resources] [%s] Resource not present on filesystem! [%s]", id.data(), path.data());
	}
	return INVALID;
}

template <typename T>
T* Resources::Find(HResource handle) const
{
	static_assert(std::is_base_of<Resource, T>::value, "T must derive from Resource!");
	return (handle > INVALID) ? m_factory.template Find<T>(handle) : nullptr;
}
} // namespace ME
