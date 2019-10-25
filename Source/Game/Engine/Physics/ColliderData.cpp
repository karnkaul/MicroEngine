#include "ColliderData.h"

namespace ME
{
const AABBData AABBData::One = AABBData(Vector2::One);
const CircleData CircleData::One = CircleData(Fixed::One, Vector2(0, 0));

AABBData::AABBData(Vector2 lowerBound, const Vector2 upperBound) : lowerBound(std::move(lowerBound)), upperBound(std::move(upperBound)) {}

AABBData::AABBData(Vector2 size) : AABBData(-Fixed::OneHalf * size, Fixed::OneHalf * size) {}

bool AABBData::Intersecting(const AABBData&) const
{
	return false;
}

bool AABBData::IsPointInRect(Vector2) const
{
	return false;
}

CircleData::CircleData(Fixed radius, Vector2 centre) : centre(std::move(centre)), radius(std::move(radius)) {}

bool CircleData::IsIntersecting(const CircleData&) const
{
	return false;
}

bool CircleData::IsPointInCircle(Vector2) const
{
	return false;
}
} // namespace ME
