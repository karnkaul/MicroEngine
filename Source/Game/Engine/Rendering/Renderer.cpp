#include "SFML/Graphics.hpp"
#include "Renderer.h"
#include "Engine/SFTypes.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Viewport/Viewport.h"

namespace ME
{
TextData::TextData() = default;
TextData::TextData(const char* szText)
{
	oText = szText;
}

TextData::TextData(std::string text)
{
	oText = text;
}

void Primitive::Instantiate(Type type)
{
	switch (type)
	{
	case Type::Rectangle:
	{
		m_uD = std::make_unique<sf::RectangleShape>();
		break;
	}
	case Type::Circle:
	{
		m_uD = std::make_unique<sf::CircleShape>();
		break;
	}
	case Type::Text:
	{
		m_uD = std::make_unique<sf::Text>();
		break;
	}
	case Type::Sprite:
	{
		m_uD = std::make_unique<sf::Sprite>();
		break;
	}
	}
	m_pT = dynamic_cast<sf::Transformable*>(m_uD.get());
}

Primitive* Primitive::SetText(const TextData& data)
{
	auto pText = CastDrawable<sf::Text>();
	if (pText)
	{
		if (data.oCharSize)
		{
			pText->setCharacterSize(*data.oCharSize);
		}
		if (data.opFont && *data.opFont)
		{
			pText->setFont((*data.opFont)->m_font);
		}
		if (data.oText)
		{
			pText->setString(*data.oText);
		}
		auto rect = pText->getLocalBounds();
		sf::Vector2f c = {rect.width * 0.5f, rect.height * 0.5f};
		pText->setOrigin(c);
	}
	return this;
}

Primitive* Primitive::SetShape(const ShapeData& data)
{
	auto pCircle = CastDrawable<sf::CircleShape>();
	auto pRect = CastDrawable<sf::RectangleShape>();
	if (data.oSize)
	{
		if (pCircle)
		{
			pCircle->setRadius((data.oSize->x + data.oSize->y).ToF32());
			pCircle->setOrigin({data.oSize->x.ToF32(), data.oSize->y.ToF32()});
		}
		if (pRect)
		{
			pRect->setSize(Cast(*data.oSize));
			sf::Vector2f o = Cast((*data.oSize * Fixed::OneHalf));
			pRect->setOrigin(o);
		}
	}
	if (data.oFill)
	{
		if (pCircle)
		{
			pCircle->setFillColor(Cast(*data.oFill));
		}
		if (pRect)
		{
			pRect->setFillColor(Cast(*data.oFill));
		}
	}
	if (data.oOutline)
	{
		if (pCircle)
		{
			pCircle->setOutlineColor(Cast(*data.oOutline));
		}
		if (pRect)
		{
			pRect->setOutlineColor(Cast(*data.oOutline));
		}
	}
	return this;
}

Primitive* Primitive::SetPosition(Vector2 world)
{
	if (m_pT)
	{
		m_pT->setPosition({world.x.ToF32(), -world.y.ToF32()});
	}
	return this;
}

void Primitive::Draw(Viewport& viewport)
{
	if (m_uD)
	{
		viewport.draw(*m_uD);
	}
}

Renderer::Renderer() = default;
Renderer::~Renderer()
{
	Clear();
}

void Renderer::Clear()
{
	m_factory.Clear();
}

HPrim Renderer::New()
{
	return m_factory.New<Primitive>();
}

Primitive* Renderer::Find(HPrim handle)
{
	return m_factory.Find<Primitive>(handle);
}

void Renderer::Render(Viewport& viewport)
{
	std::vector<Primitive*> prims;
	prims.reserve(m_factory.m_instanced.size());
	for (auto& kvp : m_factory.m_instanced)
	{
		prims.push_back(&kvp.second);
	}
	std::sort(prims.begin(), prims.end(), [](Primitive* pLHS, Primitive* pRHS) { return pLHS->m_layer < pRHS->m_layer; });
	viewport.clear();
	for (auto pPrim : prims)
	{
		pPrim->Draw(viewport);
	}
	viewport.display();
}
} // namespace ME
