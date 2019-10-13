#include "SFML/Graphics.hpp"
#include "Primitive.h"
#include "Engine/SFTypes.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Viewport/Viewport.h"

namespace ME
{
Primitive::Primitive() = default;
Primitive::Primitive(Primitive&&) = default;
Primitive& Primitive::operator=(Primitive&&) = default;

Primitive::~Primitive()
{
	if (m_uD)
	{
		LOG_D("[Prim_%d] %s destroyed", m_handle, Typename(*this).data());
	}
}

Primitive* Primitive::Instantiate(Type type)
{
	std::string logText = "[Prim_";
	logText += std::to_string(m_handle);
	logText += "] ";
	switch (type)
	{
	case Type::Rectangle:
	{
		m_uD = std::make_unique<sf::RectangleShape>();
		logText += "Rectangle";
		break;
	}
	case Type::Circle:
	{
		m_uD = std::make_unique<sf::CircleShape>();
		logText += "Circle";
		break;
	}
	case Type::Text:
	{
		m_uD = std::make_unique<sf::Text>();
		logText += "Text";
		break;
	}
	case Type::Sprite:
	{
		m_uD = std::make_unique<sf::Sprite>();
		logText += "Sprite";
		break;
	}
	}
	m_pT = dynamic_cast<sf::Transformable*>(m_uD.get());
	LOG_D("%s instantiated", logText.data());
	return this;
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
		if (data.oFill)
		{
			pText->setFillColor(Cast(*data.oFill));
		}
		if (data.oOutline)
		{
			pText->setOutlineColor(Cast(*data.oOutline));
		}
		if (data.oBorder)
		{
			pText->setOutlineThickness(Cast(*data.oBorder));
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
	if (data.oBorder)
	{
		if (pCircle)
		{
			pCircle->setOutlineThickness(Cast(*data.oBorder));
		}
		if (pRect)
		{
			pRect->setOutlineThickness(Cast(*data.oBorder));
		}
	}
	return this;
}

void Primitive::Update()
{
	if (m_pT)
	{
		m_pT->setPosition(Cast(WorldToSFML(m_position)));
		m_pT->setRotation(WorldToSFML(Vector2::ToOrientation(m_orientation)).ToF32());
	}
}

void Primitive::Draw(Viewport& viewport)
{
	if (m_uD)
	{
		viewport.draw(*m_uD);
	}
}
} // namespace ME
