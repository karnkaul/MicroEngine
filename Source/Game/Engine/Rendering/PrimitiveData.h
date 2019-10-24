#pragma once
#include <optional>
#include "GameTypes.h"
#include "Colour.h"

namespace ME
{
struct TexCoords
{
	Vector2 size;
	Fixed left;
	Fixed width;
	Fixed top;
	Fixed height;

	TexCoords();
	TexCoords(Vector2 size);

	void SetUV(Fixed u_left, Fixed v_left, Fixed u_right, Fixed v_right);
};

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
	std::optional<TexCoords> oTexCoords;
	std::optional<Vector2> oScale;
	std::optional<Colour> oFill;

	SpriteData();
	SpriteData(Texture& texture);
};

inline TexCoords::TexCoords() = default;
inline TexCoords::TexCoords(Vector2 size) : size(size) {}

inline void TexCoords::SetUV(Fixed u_left, Fixed v_left, Fixed u_right, Fixed v_right)
{
	Assert(size != Vector2::Zero, "TexCoords size not set!");
	left = u_left * size.x;
	width = u_right * size.x - left;
	top = v_left * size.y;
	height = v_right * size.y - top;
}

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
