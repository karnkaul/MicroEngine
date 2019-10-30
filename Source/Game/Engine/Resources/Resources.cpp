#include <fstream>
#include "Resources.h"
#include "GameLoop/OS.h"

namespace ME
{
std::vector<Font*> g_defaultFonts;

std::string Resources::s_resourcesPath = "Resources";

Resources::Resources() = default;
Resources::~Resources() = default;

bool Resources::Init(std::initializer_list<std::string> defaultFonts)
{
	for (const auto& fontID : defaultFonts)
	{
		auto handle = Load<Font>(fontID);
		if (handle == INVALID_HANDLE)
		{
			return false;
		}
		else
		{
			if (auto pFont = Find<Font>(handle))
			{
				g_defaultFonts.push_back(pFont);
			}
		}
	}
	return true;
}

void Resources::Clear()
{
	g_defaultFonts.clear();
	m_idToHandle.clear();
	m_factory.Clear();
}

std::string Resources::ResourcePath(const std::string& id) const
{
	std::string ret = OS::FullPath(s_resourcesPath);
	ret += "/";
	ret += id;
	return ret;
}

bool Resources::IsFilePresent(const std::string& path) const
{
	return std::ifstream(path.c_str()).good();
}

HRes Resources::FindID(const std::string& id) const
{
	auto search = m_idToHandle.find(id);
	if (search != m_idToHandle.end())
	{
		return search->second;
	}
	return INVALID_HANDLE;
}

#if defined(DEBUGGING)
bool Resources::Unload(const std::string& id)
{
	auto search = m_idToHandle.find(id);
	if (search != m_idToHandle.end())
	{
		auto handle = search->second;
		m_idToHandle.erase(search);
		if (m_factory.Destroy(handle))
		{
			LOG_I("-- [Resources] Unloaded [%s]", id.data());
		}
	}
	return false;
}
#endif
} // namespace ME
