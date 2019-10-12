#include <fstream>
#include "Resources.h"

namespace ME
{
std::string Resources::s_resourcesPath;

Resources::Resources() = default;
Resources::~Resources() = default;

bool Resources::Init(std::initializer_list<std::string> defaultFonts)
{
	for (const auto& fontID : defaultFonts)
	{
		if (Load<Font>(fontID) == INVALID_HANDLE)
		{
			return false;
		}
	}
	return true;
}

void Resources::Clear()
{
	m_idToHandle.clear();
	m_factory.Clear();
}

std::string Resources::ResourcePath(const std::string& id) const
{
	std::string ret = s_resourcesPath;
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
} // namespace ME
