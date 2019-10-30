#pragma once
#include <optional>
#include "GameTypes.h"

namespace ME
{
enum class ViewportStyle
{
	Default = 0,
	Bordlerless
};

struct ViewportSize
{
	u32 width = 0;
	u32 height = 0;

	ViewportSize() = default;
	ViewportSize(u32 width, u32 height);

	std::string ToString() const;
};

struct ViewportData
{
	ViewportSize viewportSize;
	std::string title;
	ViewportStyle style = ViewportStyle::Default;
	u8 aaLevel = 8;
	u16 maxFPS = 240;
	bool bVsync = true;
};

struct ViewportRecreateData
{
	std::optional<ViewportSize> oViewportSize;
	std::optional<ViewportStyle> oStyle;
	std::optional<std::string> oTitle;
};
} // namespace ME
