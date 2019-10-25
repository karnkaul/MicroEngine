#pragma once
#include "GameTypes.h"

namespace ME
{
// \brief Wrapper for Axis Aligned Bounding Box Collider data.
// Provides APIs to test intersection and point presence
struct AABBData
{
	static const AABBData One;

	Vector2 lowerBound;
	Vector2 upperBound;

	AABBData() = default;
	AABBData(Vector2 size);
	AABBData(Vector2 lowerBound, Vector2 upperBound);

	bool Intersecting(const AABBData& other) const;
	bool IsPointInRect(Vector2 point) const;
};

// \brief Wrapper for Circle Collider data.
// Provides APIs to test intersection and point presence
struct CircleData
{
	static const CircleData One;

	Vector2 centre;
	Fixed radius;

	CircleData() = default;
	CircleData(Fixed radius, Vector2 centre);

	bool IsIntersecting(const CircleData& other) const;
	bool IsPointInCircle(Vector2 point) const;
};
} // namespace ME
