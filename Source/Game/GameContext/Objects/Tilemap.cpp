#include "Engine/GameServices.h"
#include "Engine/Rendering/LayerIDs.h"
#include "Engine/Rendering/Primitive.h"
#include "Engine/Resources/Resource.h"
#include "Tilemap.h"

namespace ME
{
void Tilemap::OnRespawn()
{
	Core::RemoveIf<HPrim>(m_prims, [](auto hPrim) { return g_pRenderer->Find(hPrim) == nullptr; });
	for (auto hPrim : m_prims)
	{
		g_pRenderer->Find(hPrim)->m_bEnabled = true;
	}
}

void Tilemap::OnDestroy()
{
	Clear();
}

void Tilemap::OnDespawn()
{
	Core::RemoveIf<HPrim>(m_prims, [](auto hPrim) { return g_pRenderer->Find(hPrim) == nullptr; });
	for (auto hPrim : m_prims)
	{
		g_pRenderer->Find(hPrim)->m_bEnabled = false;
	}
}

void Tilemap::FillSpace(Vector2 space, Texture& texture)
{
	u16 xCols = 0, xRows = 0;
	Vector2 texSize = texture.Size();
	Fixed xResidue = space.x - texSize.x;
	while (xResidue > Fixed::Zero)
	{
		++xCols;
		xResidue -= (2 * texSize.x);
	}
	Fixed yResidue = space.y - texSize.y;
	while (yResidue > Fixed::Zero)
	{
		++xRows;
		yResidue -= (2 * texSize.y);
	}
	Clear();
	Fixed x, y;
	size_t idx = 0;
	auto create = [&](Vector2 centre) {
		Primitive* pPrim = nullptr;
		if (idx < m_prims.size())
		{
			pPrim = g_pRenderer->Find(m_prims[idx]);
		}
		else
		{
			auto hPrim = g_pRenderer->New();
			pPrim = g_pRenderer->Find(hPrim);
			pPrim->Instantiate(Primitive::Type::Sprite);
			m_prims.push_back(hPrim);
		}
		++idx;
		Assert(pPrim, "Primitive is null!");
		SpriteData sData;
		sData.pTexture = &texture;
		pPrim->SetSprite(sData);
		pPrim->m_layer = Layers::L0050_BG;
		pPrim->m_position = centre;
		pPrim->m_bEnabled = true;
	};
	auto createRow = [&]() {
		x = Fixed::Zero;
		create({x, y});
		for (u16 col = 1; col <= xCols; ++col)
		{
			x = col * texSize.x;
			create({x, y});
			create({-x, y});
		}
	};
	// Centre
	createRow();
	for (u16 row = 1; row <= xRows; ++row)
	{
		y = row * texSize.y;
		createRow();
		y = -y;
		createRow();
	}
}

void Tilemap::Clear()
{
	for (auto hPrim : m_prims)
	{
		g_pRenderer->Destroy(hPrim);
	}
	m_prims.clear();
}
} // namespace ME
