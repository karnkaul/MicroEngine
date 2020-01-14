#include <thread>
#include "Engine/GData.h"
#include "Engine/GameServices.h"
#include "Controller.h"
#include "Bubble.h"

namespace ME
{
void Controller::OnCreate()
{
	// This function does not exist outside DEBUGGING, so its use must be guarded!
	// Also we don't want to do this except when in Debug anyway.
#if defined(DEBUGGING)
	// Unloading this asset allows modification of the file on-the-fly,
	// while the game is running, and simply recreating this object / reloading the world.
	g_pResources->Unload("Tutorials/T4_Controller.json");
#endif
	auto hControllerData = g_pResources->Load<Text>("Tutorials/T4_Controller.json");
	if (auto pText = g_pResources->Find<Text>(hControllerData))
	{
		// New class: GData, like a really dumb JSON
		const Core::GData gData(pText->m_text);
		// Obtain the type information from data instead of code
		std::string typeText = gData.GetString("type");
		if (!typeText.empty())
		{
			Strings::ToLower(typeText);
			if (typeText == "immediate")
			{
				m_type = Type::Immediate;
			}
			else if (typeText == "interpolated")
			{
				m_type = Type::Interpolated;
			}
		}
		// GData can take a default value that it will return back
		// if a key is not found
		m_bClampToWorld = gData.GetBool("clampToWorld", true);
		m_speed = Fixed(gData.GetF64("speed", 1.0));
		// Try modifying all these values on the fly!
	}
	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			// We want a unit vector pointing in some direction, as if the player
			// were moving an analogue stick; for keybnoard it's just one of eight.
			Vector2 delta;
			if (frame.IsHeld({KeyCode::A, KeyCode::Left}))
			{
				delta += {-1, 0};
			}
			// Not using `else if` allows these to cancel out if both keys pressed instead of the "which 'if' was first" problem
			if (frame.IsHeld({KeyCode::D, KeyCode::Right}))
			{
				delta += {1, 0};
			}
			if (frame.IsHeld({KeyCode::Up, KeyCode::W}))
			{
				delta += {0, 1};
			}
			if (frame.IsHeld({KeyCode::Down, KeyCode::S}))
			{
				delta += {0, -1};
			}
			// Normalise it (this is expensive), otherwise diagonal displacement will be 1.414 times axial;
			// we want the change to be consistent in all directions
			delta.Normalise();
			// Add the delta to the current target
			m_targetVelocity += delta;

			// Note: this is not wrapped in `DEBUGGING`, so it will also occur for Release builds!
			if (frame.IsHeld(KeyCode::Backspace))
			{
				// !! DEMONSTRATION CODE ONLY!! DO NOT CALL SLEEP ANYWHERE YOURSELF !!
				// Deliberately take way too much time before returning;
				// This will cause a massive drop in FPS, and the engine
				// will start throwing warnings about slowed-down time
				// Try using different durations to see what happens : until 30 FPS
				// the engine will cope, below that it will start throwing away chunks of time
				// and the game will slow down
				const auto SHORT = std::chrono::milliseconds(25);
				const auto LONG = std::chrono::milliseconds(50);
				auto duration = SHORT;
				// Make LCtrl / RCtrl + Backspace slow down even more!
				if (frame.IsHeld({KeyCode::LControl, KeyCode::RControl}))
				{
					duration = LONG;
				}
				std::this_thread::sleep_for(duration);
			}

			return false;
		},
		true);
}

void Controller::Tick(Time dt)
{
	switch (m_type)
	{
	case Type::Immediate:
	{
		// Scale target (unit vector) by three and set that as current velocity
		m_velocity = 3 * m_targetVelocity;
		break;
	}
	case Type::Interpolated:
	{
		// New function: this takes in a min and max and a normalised "alpha" [0, 1],
		// and returns a linear interpolation (LERP) of min/max using alpha.
		// Eg: Lerp(0, 10, 0.5) = 5; Lerp(0, 10, 0.8) = 8
		// Using the velocity itself as the min makes it progressively move towards 3 * targetVelocity;
		// the rate of "catching up" depends on the value of alpha, which needs to be
		// scaled by `dt` again, to remain framerate independent.
		m_velocity = Maths::Lerp(m_velocity, 3 * m_targetVelocity, Fixed(dt.AsSeconds() * 20));
		break;
	}
	}
	// Compute displacement (velocity * time) (notice `dt` has effectively been multiplied *twice*)
	// Verify framerate independence by using Backspace / Ctrl + Backspace while moving and observing
	// both FPS output and if/how the movement/rendering changes based on high `dt`.
	// Also try removing `dt` scaling here to deliberately make movement framerate dependent, just so
	// you know how to recognise it when you see such a bug the next time.
	const Vector2 displacement = Fixed(dt.AsSeconds()) * m_speed * m_velocity * 500;
	Vector2 position = m_transform.Position() + displacement;
	if (m_bClampToWorld)
	{
		// World limits are [-0.5 * size, 0.5 * size] on each axis
		const Vector2 worldLimits = Fixed::OneHalf * g_pGFX->WorldSpace();
		// Compute max x and y (limit - padding)
		// Bounds() returns the bounds of whatever Primitive has been instantiated
		// TopRight() is a shortcut for Size() * 0.5, since the origin is in the centre
		const Vector2 xyPad = Bounds().TopRight();
		const Fixed maxX = worldLimits.x - xyPad.x;
		const Fixed maxY = worldLimits.y - xyPad.y;
		// New function: Clamp => much simpler than lerp, returns value if within [min, max] else either boundary
		position.x = Maths::Clamp(position.x, -maxX, maxX);
		position.y = Maths::Clamp(position.y, -maxY, maxY);
	}
	m_transform.SetPosition(position);
	// Reset target (we aren't dealing with acceleration; in fact we cannot, without a fixed-time loop)
	m_targetVelocity = Vector2::Zero;

	ShapeData data;
	data.oSize = {m_radius, m_radius};
	SetShape(data);

	GameObject::Tick(dt);
}
} // namespace ME
