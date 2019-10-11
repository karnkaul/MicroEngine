#include <fstream>
#include "Resources.h"

namespace ME
{
Resource::Resource() = default;
Resource::~Resource()
{
	if (m_bOK)
	{
		LOG_I("== [%s] %s destroyed", m_id.data(), m_type.data());
	}
}

bool Resource::Load(std::string id, std::string path)
{
	m_id = std::move(id);
	m_path = std::move(path);
	return OnLoad();
}

Font::Font()
{
	m_type = "Font";
}

Font::~Font() = default;

bool Font::OnLoad()
{
	return m_bOK = m_font.loadFromFile(m_path);
}

std::string Resources::s_resourcesPath;
Resources* g_pResources = nullptr;

Resources::Resources()
{
	Assert(g_pResources == nullptr, "Global pointer already registered!");
	g_pResources = this;
}
Resources::~Resources()
{
	g_pResources = nullptr;
}

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

Resources::HResource Resources::FindID(const std::string& id) const
{
	auto search = m_idToHandle.find(id);
	if (search != m_idToHandle.end())
	{
		return search->second;
	}
	return INVALID_HANDLE;
}
} // namespace ME
