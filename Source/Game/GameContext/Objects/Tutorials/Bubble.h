#pragma once
#include "../GameObject.h"

namespace ME
{
class Bubble : public GameObject
{
public:
	// Expose all the properties as if its a struct, no need for pointless getters/setters
	Colour m_colour = Colour::Yellow;
	Colour m_outline = Colour::Black;
	Fixed m_diameter = 20;
	// Implementation adjusts its computation such that exposed value is a nice "1"
	Fixed m_ySpeed = Fixed::One;
	Time m_ttl = Time::Seconds(1.0f);

protected:
	// Similar to `GameWorld` derived objects, but without `Start()`/`Stop()`;
	// `GameObject`s "start" when they are created, and "stop" on destruction
	void OnCreate() override;
	void Tick(Time dt) override;
};
} // namespace ME
