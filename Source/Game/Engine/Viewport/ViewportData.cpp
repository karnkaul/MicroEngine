#include "ViewportData.h"

namespace ME
{
ViewportSize::ViewportSize(u32 width, u32 height) : width(width), height(height) {}

std::string ViewportSize::ToString() const
{
	std::string ret;
	ret.reserve(12);
	ret += "[";
	ret += std::to_string(width);
	ret += "x";
	ret += std::to_string(height);
	ret += "]";
	return ret;
}
} // namespace ME
