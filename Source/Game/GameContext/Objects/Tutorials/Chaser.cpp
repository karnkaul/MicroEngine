#include "Engine/GameServices.h"
#include "Chaser.h"

namespace ME
{
void Chaser::OnCreate()
{
	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			m_targetPos = frame.mouseInput.worldPosition;
			if (frame.IsReleased(KeyCode::Tab))
			{
				m_state = (m_state == State::Chasing) ? State::Idle : State::Chasing;
			}
			return false;
		},
		true);
}

void Chaser::Tick(Time dt)
{
	// Compute delta rotation (positive is counter-clockwise)
	const auto dr = Fixed(dt.AsSeconds()) * m_omega * 150;
	// Update orientation
	switch (m_state)
	{
	case State::Idle:
	{
		Idle(dr);
		break;
	}
	case State::Chasing:
	{
		Chase(dr);
		break;
	}
	}
	// Move along (updated) orientation
	const Vector2 displacement = Fixed(dt.AsSeconds()) * m_transform.Orientation() * m_speed * 200;
	m_transform.SetPosition(m_transform.Position() + displacement);

	GameObject::Tick(dt);
}

void Chaser::Idle(Fixed dRot)
{
	// Just keep rotating steadily
	m_transform.SetOrientation(Vector2::Rotate(m_transform.Orientation(), dRot));
}

void Chaser::Chase(Fixed dRot)
{
	// Get a normalised vector to the target
	const Vector2 nTarget = (m_targetPos - m_transform.Position()).Normalised();
	const Vector2 nSelf = m_transform.Orientation();
	// Project self onto target (FYI: dot products are commutative scalars: A.B == B.A)
	const Fixed proj = nSelf.Dot(nTarget);
	// Stop rotating if projection is one (will never be, which is why movement is shaky)
	if (Maths::Abs(proj) < Fixed::One)
	{
		// We need the normal in order to determine which direction to rotate in: the sign of its projection
		// will vary based on which side of `nSelf` `nTarget` lies (cos is positive in both 1st and 4th quadrants)
		// So, compute Y-component of the orientation vector and dot *that* against nTarget.
		const Vector2 normal = Vector2::Rotate(nSelf, 90);
		// Why this roundabout way and not just `normalProj = proj.ArcCos().Sin()`?
		// Because trig functions only work in 180 degrees, we need full 360 degree resolution,
		// and vectors are sufficient to do that (at least with one DOF / in 2D space)

		// If the normal projection is negative, the target is > 180 degrees away, so rotate in the other direction
		if (nTarget.Dot(normal) < Fixed::Zero)
		{
			dRot = -dRot;
		}
		m_transform.SetOrientation(Vector2::Rotate(nSelf, dRot));
	}
}
} // namespace ME
