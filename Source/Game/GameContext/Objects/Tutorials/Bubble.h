#pragma once
#include "../GameObject.h"

namespace ME
{
class Bubble : public GameObject
{
public:
	Colour m_colour = Colour::Yellow;
	Colour m_outline = Colour::Black;
	Fixed m_diameter = 20;
	Fixed m_ySpeed = Fixed::One;
	Time m_ttl = Time::Seconds(1.0f);

protected:
	void OnCreate() override;
	void Tick(Time dt) override;
};
} // namespace ME