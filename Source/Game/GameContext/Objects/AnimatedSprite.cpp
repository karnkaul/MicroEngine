#include "AnimatedSprite.h"

namespace ME
{
void AnimatedSprite::Tick(Time dt)
{
	if (m_bAnimate)
	{
		bool bNewIdx = m_sheet.Tick(dt);
		if (m_bSelfDestruct && bNewIdx && m_sheet.m_index == 0)
		{
			Destroy();
		}
		else
		{
			SpriteData data;
			data.oTexCoords = m_sheet.m_texCoords[m_sheet.m_index];
			SetSprite(data);
		}
	}
}

void AnimatedSprite::ResetFrame()
{
	m_sheet.Reset(false);
}
} // namespace ME
