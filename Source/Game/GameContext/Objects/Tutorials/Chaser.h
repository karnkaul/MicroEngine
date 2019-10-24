#pragma once
#include "../GameObject.h"

namespace ME
{
// Base class that contains all the chasing logic and API but nothing about rendering;
// either owning World or a derived class is expected to handle that.
class Chaser : public GameObject
{
public:
	// Always use `enum class` and not juse `enum`
	// This is an enum declared within an existing class, so
	// outside the class it must be accessed via `Chaser::State`.
	enum class State
	{
		Idle = 0,
		Chasing
	};

public:
	Fixed m_speed = Fixed::One;
	// Angular speed
	Fixed m_omega = Fixed::One;
	State m_state = State::Idle;
	// Where to go when chasing
	Vector2 m_targetPos;

protected:
	// Similar to `GameWorld` derived objects, but without `Start()`/`Stop()`;
	// `GameObject`s "start" when they are created, and "stop" on destruction.
	void OnCreate() override;
	void Tick(Time dt) override;

private:
	// These are private because even derived classes shouldn't be able to
	// call them directly, they're sub-parts of Tick.
	void Idle(Fixed dRot);
	void Chase(Fixed dRot);
};
} // namespace ME
