#pragma once
#include "GameObject.h"
#include "Collision.h"

namespace ME
{
class Projectile : public GameObject
{
public:
	Fixed m_speed = Fixed::One;
	Time m_ttl = Time::Seconds(2.0f);

protected:
	Time m_elapsed;

protected:
	void OnCreate() override;
	void OnRespawn() override;
	void Tick(Time dt) override;

public:
	bool OnHit(Collision::Info info);
};
} // namespace ME
