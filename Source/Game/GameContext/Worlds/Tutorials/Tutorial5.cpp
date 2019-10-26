#include "Engine/GameServices.h"
#include "Engine/Physics/ColliderData.h"
#include "../../ObjectPool.h" // Check out this class first
#include "../../Objects/Tutorials/Bubble.h"
#include "../../Objects/Tutorials/Projectile.h" // Check out this class first
#include "../../Objects/Tutorials/Rocket.h"		// Check out this class first
#include "Tutorial5.h"

namespace ME
{
void Tutorial5::OnCreated()
{
	// A Texture is an image that lives on the GPU memory; it is extremely
	// fast and performant when in use, but is *very* slow to create.
	// Check the texture out in Resources: you'll notice it faces right; this
	// is because by convention our "default" orientation in the world is `Vector2::Right` (1, 0).
	g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	g_pResources->Load<Texture>("Textures/Exhaust_128x128_2x4.png");
}

void Tutorial5::OnStarting()
{
	RegisterInput([this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Temp");
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial4");
		}
		// You can pass KeyType enums also
		else if (frame.IsReleased(KeyType::MOUSE_BTN_0) || frame.IsReleased(KeyCode::LControl))
		{
			// Fire a new projectile!
			std::string name = "Projectile_";
			name += std::to_string(m_projectileCount++);
			// Use the pool instead of instantiating a whole new GameObject
			if (auto pPool = FindPool(m_hProjectiles))
			{
				auto pProjectile = pPool->NewObject(std::move(name));
				if (pProjectile)
				{
					// Set its transform; it doesn't even need to be cast into a Projectile*,
					// since transform is part of GameObject itself.
					if (auto pRocket = FindObject<Rocket>(m_hRocket))
					{
						const auto& t = pRocket->m_transform;
						pProjectile->m_transform.SetOrientation(t.WorldOrientation());
						Vector2 pos = t.WorldPosition() + (t.WorldOrientation() * 75);
						pProjectile->m_transform.SetPosition(pos);
						pProjectile->SetEnabled(true);
					}
				}
			}
		}
		return false;
	});

	m_hMainText = NewObject<GameObject>("MainText");
	if (auto pText = FindObject<GameObject>(m_hMainText))
	{
		pText->Instantiate(Primitive::Type::Text);
		pText->SetText("Press [Tab] to toggle chasing");
	}
	m_hRocket = NewObject<Rocket>("Rocket");
	if (auto pRocket = FindObject<Rocket>(m_hRocket))
	{
		// Update some configuration variables
		pRocket->m_speed = 2;
		pRocket->m_layer = 100;
	}
	// Note: Press [Shift] + [c] to toggle collider debug shapes

	// This is an Object Pool, which is what would be the right way to spawn
	// new bubbles in Tutorial3. The way to use it is to first set up the "generator":
	// a function that instantiates a new GameObject (or derived type) and returns a pointer
	// to it. The pool will use this to create new objects (if none are available).
	m_bubbleCount = 0;
	m_hBubbles = NewPool();
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		pBubbles->SetGenerator([this]() -> GameObject* {
			// We want to set up everything related to Bubbles except their transform, which will be done
			// per instance obtained from the pool at runtime (not this code, the one in RegisterInput).
			std::string name = "Bubble_";
			name += std::to_string(m_bubbleCount++);
			auto handle = NewObject<Bubble>(std::move(name));
			if (auto pBubble = FindObject<Bubble>(handle))
			{
				// Set a random time to live
				const s32 ttlSecs = Maths::Random::Range(8, 10);
				pBubble->m_ttl = Time::Seconds(static_cast<f32>(ttlSecs));
				// Scale the size and speed proportional to its ttl
				const Fixed nSize = Fixed(ttlSecs) / Fixed(10);
				pBubble->m_diameter += (nSize * 20);
				pBubble->m_ySpeed -= (nSize * Fixed::OneHalf);
				// Add a Circle collider to this Collision component (with no callback)
				pBubble->GetCollision().AddCircle(nullptr, pBubble->m_diameter);
				pBubble->m_layer = 10;
				return pBubble;
			}
			// Lambda requires a GameObject* return, so return null here
			return nullptr;
		});
		// PreInstantiate will instantiate the number of objects passed, using the given string as a
		// name prefix. This prevents the need to have to instantiate new objects later (no allocations
		// during hot game-time is extremely desriable, though not always achievable).
		pBubbles->PreInstantiate("Bubble", 3);
	}

	// Same thing for projectiles
	m_projectileCount = 0;
	m_hProjectiles = NewPool();
	if (auto pProjectiles = FindPool(m_hProjectiles))
	{
		pProjectiles->SetGenerator([this]() -> GameObject* {
			std::string name = "Projectile_";
			name += std::to_string(m_projectileCount++);
			auto handle = NewObject<Projectile>(std::move(name));
			if (auto pProjectile = FindObject<Projectile>(handle))
			{
				pProjectile->Instantiate(Primitive::Type::Rectangle);
				ShapeData data;
				data.oSize = {75, 6};
				data.oFill = Colour::Cyan;
				pProjectile->SetShape(data).m_layer = 110;
				// Add an AABB collider to this Collision component (with no callback)
				auto token = pProjectile->GetCollision().AddAABB(
					[this, handle](Collision::Info info) { FindObject<Projectile>(handle)->OnHit(info); }, AABBData({20, 20}));
				m_collisionTokens.push_back(token);
				if (auto pRocket = FindObject<Rocket>(m_hRocket))
				{
					// Setup the speed here itself, since we want all instances to share that property
					pProjectile->m_speed = pRocket->m_speed * 5;
				}
				return pProjectile;
			}
			return nullptr;
		});
		pProjectiles->PreInstantiate("Projectile", 20);
	}
}

void Tutorial5::Tick(Time dt)
{
	if (auto pRocket = FindObject<Chaser>(m_hRocket))
	{
		SpriteData data;
		// Override only the colour, based on Rocket's state
		data.oFill = pRocket->m_state == Chaser::State::Chasing ? Colour::Red : Colour::White;
		pRocket->SetSprite(data);
	}
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		if (pBubbles->Spawned() < 3)
		{
			std::string name = "Bubble_";
			name += std::to_string(m_bubbleCount++);
			auto pBubble = pBubbles->NewObject(std::move(name));
			// Set a random x in the world
			const Fixed nX = Maths::Random::Range(-Fixed::One, Fixed::One);
			// Y near the bottom
			const Fixed nY = -Fixed(0.8f);
			pBubble->m_transform.SetPosition(g_pGFX->WorldProjection({nX, nY}));
			pBubble->SetEnabled(true);
		}
	}
	GameWorld::Tick(dt);
}

void Tutorial5::OnStopping()
{
	m_hBubbles = m_hProjectiles = m_hRocket = m_hMainText = INVALID_HANDLE;
}
} // namespace ME
