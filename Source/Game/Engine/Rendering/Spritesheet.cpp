#include "Engine/SFTypes.h"
#include "Spritesheet.h"

namespace ME
{
void Spritesheet::Autobuild(Texture& texture, u16 columns, u16 rows)
{
	Assert(columns > 0 && rows > 0, "Division by zero!");
	m_pTexture = &texture;
	const Vector2 texSize = texture.Size();
	const Fixed w = texSize.x / columns;
	const Fixed h = texSize.y / rows;
	m_texCoords.clear();
	for (u16 row = 0; row < rows; ++row)
	{
		Fixed y = h * row;
		for (u16 col = 0; col < columns; ++col)
		{
			TexCoords coords(texSize);
			coords.left = w * col;
			coords.top = y;
			coords.width = w;
			coords.height = h;
			m_texCoords.push_back(coords);
		}
	}
	m_index = 0;
}

bool Spritesheet::Tick(Time dt)
{
	if (m_period > Time::Zero)
	{
		m_elapsed += dt;
		const Fixed scale = Fixed(1, ToS32(m_texCoords.size()));
		const Time subPeriod = m_period.Scaled(scale);
		if (m_elapsed >= subPeriod)
		{
			m_elapsed = Time::Zero;
			m_index = (m_index == m_texCoords.size() - 1) ? 0 : m_index + 1;
			return true;
		}
	}
	return false;
}

void Spritesheet::Reset(bool bClearTexCoords)
{
	m_index = 0;
	m_elapsed = Time::Zero;
	if (bClearTexCoords)
	{
		m_texCoords.clear();
	}
}
} // namespace ME
