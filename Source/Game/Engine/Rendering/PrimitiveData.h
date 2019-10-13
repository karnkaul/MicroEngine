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

	TextData();
	TextData(const char* szText);
	TextData(std::string text);
};

struct ShapeData
{
	std::optional<Vector2> oSize;
	std::optional<Colour> oFill;
	std::optional<Colour> oOutline;
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

} // namespace ME
