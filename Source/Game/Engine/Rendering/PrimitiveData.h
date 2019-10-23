#pragma once
#include <optional>
#include "GameTypes.h"
#include "Colour.h"

namespace ME
{
struct TextData
{
	std::optional<std::string> oText;
	std::optional<u32> oCharSize;
	std::optional<class Font*> opFont;
	std::optional<Colour> oFill;
	std::optional<Colour> oOutline;
	std::optional<Fixed> oBorder;

	TextData();
	TextData(const char* szText);
	TextData(std::string text);
};

struct ShapeData
{
	std::optional<Vector2> oSize;
	std::optional<Colour> oFill;
	std::optional<Colour> oOutline;
	std::optional<Fixed> oBorder;
};

struct SpriteData
{
	class Texture* pTexture = nullptr;
	std::optional<Rect2> oUV;
	std::optional<Vector2> oScale;
	std::optional<Colour> oFill;

	SpriteData();
	SpriteData(Texture& texture);
};

inline TextData::TextData() = default;
inline TextData::TextData(const char* szText)
{
	oText = szText;
}

inline TextData::TextData(std::string text)
{
	oText = text;
}

inline SpriteData::SpriteData() = default;
inline SpriteData::SpriteData(Texture& texture) : pTexture(&texture) {}
} // namespace ME
