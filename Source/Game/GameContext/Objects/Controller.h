#pragma once
#include "GameObject.h"

namespace ME
{
// A simple data-driven keyboard controller
class Controller : public GameObject
{
public:
	enum class Type
	{
		Immediate = 0, // Stupid simple
		Interpolated   // With a bit of smoothening
	};

public:
	Fixed m_speed = Fixed::One; // Coefficient of velocity
	Fixed m_radius = 50;
	Type m_type;
	bool m_bClampToWorld = true; // Prevents object from going off screen

private:
	Vector2 m_velocity;		  // Current
	Vector2 m_targetVelocity; // To get to

protected:
	void OnCreate() override;
	void Tick(Time dt) override;
};
} // namespace ME
