#pragma once
#include "../GameObject.h"

namespace ME
{
class Food : public GameObject
{
public:
	Colour m_colour = Colour::Red;
	Colour m_outline = Colour::White;
	Fixed m_diameter = 40;

protected:
	void OnCreate() override;
	void Tick(Time dt) override;

public:
	void OnHit(Collision::Info info);
};
} // namespace ME
