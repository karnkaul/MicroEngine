#include "Bubble.h"

namespace ME
{
void Bubble::OnCreate()
{
	// These are now member functions of `GameObject`!
	Instantiate(Primitive::Type::Circle);
	ShapeData data;
	data.oBorder = Fixed::One;
	SetShape(data);
}

void Bubble::Tick(Time dt)
{
	m_ttl -= dt;
	if (m_ttl <= Time::Zero)
	{
		// This bubble is ded
		Destroy();
		return;
	}

	// Push the object above by some amount (playtest and find out)
	// Scale it by `dt` so its independent of frame-rate (is larger
	// when more time elapses between frames, so overall movement
	// over time remains identical over varying framerates)
	Fixed dy = m_ySpeed * Fixed(dt.AsMilliseconds() / 5);
	Vector2 newPos = m_transform.Position() + Vector2(0, dy);
	m_transform.SetPosition(newPos);

	// Properties might have changed, update the underlying shape to match
	ShapeData data;
	data.oSize = {m_diameter, m_diameter};
	data.oFill = m_colour;
	data.oOutline = m_outline;
	SetShape(data);
	
	// Finally let the base class update the actual primitive's transform
	GameObject::Tick(dt);
}
} // namespace ME
