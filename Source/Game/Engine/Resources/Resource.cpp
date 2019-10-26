#include <fstream>
#include "Engine/SFTypes.h"
#include "Resource.h"

namespace ME
{
Resource::Resource() = default;

Resource::~Resource()
{
	if (m_bOK)
	{
		LOG_I("-- [%s] %s (Resource_%d) destroyed", m_id.data(), m_type.data(), m_handle);
	}
}

void Resource::SetHandle(HRes handle)
{
	m_handle = handle;
}

bool Resource::Load(std::string id, std::string path)
{
	m_id = std::move(id);
	m_path = std::move(path);
	m_type = Typename(*this);
	return OnLoad();
}

bool Font::OnLoad()
{
	return m_bOK = m_font.loadFromFile(m_path);
}

bool Text::OnLoad()
{
	std::ifstream file(m_path.data());
	if (file.good())
	{
		std::string line;
		while (std::getline(file, line))
		{
			m_text += line;
			m_text += "\n";
		}
		return m_bOK = true;
	}
	return false;
}

bool Texture::OnLoad()
{
	return m_bOK = m_texture.loadFromFile(m_path);
}

Vector2 Texture::Size() const
{
	return Cast(m_texture.getSize());
}
} // namespace ME
