#include "ColliderData.h"

namespace ME
{
const AABBData AABBData::One = AABBData(Vector2::One);
const CircleData CircleData::One = CircleData(Fixed::One, Vector2(0, 0));

AABBData::AABBData(Vector2 lowerBound, const Vector2 upperBound) : lowerBound(std::move(lowerBound)), upperBound(std::move(upperBound)) {}

AABBData::AABBData(Vector2 size) : AABBData(-Fixed::OneHalf * size, Fixed::OneHalf * size) {}

bool AABBData::Intersecting(const AABBData& other) const
{
	bool xInRange = (other.lowerBound.x >= this->lowerBound.x && other.lowerBound.x <= this->upperBound.x)
					|| (other.upperBound.x >= this->lowerBound.x && other.upperBound.x <= this->upperBound.x);
	bool yInRange = (other.lowerBound.y >= this->lowerBound.y && other.lowerBound.y <= this->upperBound.y)
					|| (other.upperBound.y >= this->lowerBound.y && other.upperBound.y <= this->upperBound.y);
	return xInRange && yInRange;
}

bool AABBData::IsPointInRect(Vector2 point) const
{
	bool xInRange = (point.x >= this->lowerBound.x && point.x <= this->upperBound.x);
	bool yInRange = (point.y >= this->lowerBound.y && point.y <= this->upperBound.y);
	return xInRange && yInRange;
}

CircleData::CircleData(Fixed radius, Vector2 centre) : centre(std::move(centre)), radius(std::move(radius)) {}

bool CircleData::IsIntersecting(const CircleData& other) const
{
	auto length = other.radius + this->radius;
	auto x = other.centre.x - this->centre.x;
	auto y = other.centre.y - this->centre.y;

	return (length * length) > (x * x) + (y * y);
}

bool CircleData::IsPointInCircle(Vector2 point) const
{
	auto x = point.x - this->centre.x;
	auto y = point.y - this->centre.y;

	return (this->radius * this->radius) > (x * x) + (y * y);
}
} // namespace ME
