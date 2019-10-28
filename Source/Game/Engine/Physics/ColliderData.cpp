#include "ColliderData.h"

namespace ME
{
namespace
{
bool InRange(Fixed x, Fixed lower, Fixed upper)
{
	return x >= lower && x <= upper;
}
} // namespace

const AABBData AABBData::One = AABBData(Vector2::One);
const CircleData CircleData::One = CircleData(Fixed::One, Vector2(0, 0));

AABBData::AABBData(Vector2 lowerBound, const Vector2 upperBound) : lowerBound(std::move(lowerBound)), upperBound(std::move(upperBound)) {}

AABBData::AABBData(Vector2 size) : AABBData(-Fixed::OneHalf * size, Fixed::OneHalf * size) {}

bool AABBData::Intersecting(const AABBData& other) const
{
	bool xInRange = InRange(other.lowerBound.x, lowerBound.x, upperBound.x) || InRange(other.upperBound.x, lowerBound.x, upperBound.x);
	bool yInRange = InRange(other.lowerBound.y, lowerBound.y, upperBound.y) || InRange(other.upperBound.y, lowerBound.y, upperBound.y);
	return xInRange && yInRange;
}

bool AABBData::IsPointInRect(Vector2 point) const
{
	bool xInRange = InRange(point.x, lowerBound.x, upperBound.x);
	bool yInRange = InRange(point.y, lowerBound.y, upperBound.y);
	return xInRange && yInRange;
}

CircleData::CircleData(Fixed radius, Vector2 centre) : centre(std::move(centre)), radius(std::move(radius)) {}

bool CircleData::IsIntersecting(const CircleData& other) const
{
	f64 centreDist = (other.centre - centre).SqrMagnitude();
	f64 radiusDist = (other.radius + radius).ToF64();

	return (radiusDist * radiusDist) > centreDist;
}

bool CircleData::IsPointInCircle(Vector2 point) const
{
	f64 distanceSqr = (point - centre).SqrMagnitude();
	f64 radiusDistSqr = radius.ToF64() * radius.ToF64();

	return radiusDistSqr > distanceSqr;
}
} // namespace ME
