#include "Resource.h"

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
	m_type = Typename(*this);
	return OnLoad();
}

Font::Font() = default;
Font::~Font() = default;

bool Font::OnLoad()
{
	return m_bOK = m_font.loadFromFile(m_path);
}
} // namespace ME
