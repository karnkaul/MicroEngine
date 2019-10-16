#include "Bubble.h"

namespace ME
{
void Bubble::OnCreate()
{
	Instantiate(Primitive::Type::Circle);
	ShapeData data;
	data.oBorder = Fixed::One;
	SetShape(data);
}

void Bubble::Tick(Time dt)
{
	GameObject::Tick(dt);

	m_ttl -= dt;
	if (m_ttl <= Time::Zero)
	{
		Destroy();
		return;
	}

	Fixed dy = m_ySpeed * Fixed(dt.AsMilliseconds() / 5);
	Vector2 newPos = m_transform.Position() + Vector2(0, dy);
	m_transform.SetPosition(newPos);

	ShapeData data;
	data.oSize = {m_diameter, m_diameter};
	data.oFill = m_colour;
	data.oOutline = m_outline;
	SetShape(data);
}
} // namespace ME