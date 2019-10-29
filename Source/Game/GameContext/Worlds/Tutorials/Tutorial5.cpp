#include "Engine/GameServices.h"
#include "Engine/Physics/ColliderData.h"
#include "../../ObjectPool.h" // Check out this class first
#include "../../Objects/Tilemap.h" // Check out this class first
#include "../../Objects/Tutorials/Bubble.h"
#include "../../Objects/Tutorials/Projectile.h" // Check out this class first
#include "../../Objects/Tutorials/Rocket.h"		// Check out this class first
#include "../../Objects/UI/UIButton.h"			// Check out this class first
#include "Tutorial5.h"

namespace ME
{
namespace
{
// Store World IDs and other constants here so we can reference them later
// `static` here does two things, though we only care about one:
//	1. Initialises these variables before `main()` runs, so we know they will be safe to reference in code below
//	2. Marks the variables as having "internal linkage" (no other CPP can reference them via `extern`);
//		the anonymous namespace already does that, so it's basically redundant.
static const std::string NEXT_WORLD = "Temp";
static const std::string PREV_WORLD = "Tutorial4";
constexpr u32 INIT_BUBBLES = 5;
} // namespace

void Tutorial5::OnCreate()
{
	// A Texture is an image that lives on the GPU memory; it is extremely
	// fast and performant when in use, but is *very* slow to create.
	// Check the texture out in Resources: you'll notice it faces right; this
	// is because by convention our "default" orientation in the world is `Vector2::Right` (1, 0).
	g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	g_pResources->Load<Texture>("Textures/Exhaust_128x128_2x4.png");
	m_hTile = g_pResources->Load<Texture>("Textures/PurpleBrickTile_512x512.png");
}

void Tutorial5::OnStarting()
{
	// Lambdas/closures can also be created as an "object" and passed around
	auto onInput = [this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld(NEXT_WORLD);
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld(PREV_WORLD);
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
				/**
				 * Note: in this case, `auto pProjectile = pPool->NewObject(std::move(name))`
				 * would also work fine, since the code below doesn't refer to anything particular to Projectile.
				 * Also, while the pool offers this template function for convenience, it will only work
				 * if the passed type is the same as (or parent of) what's used in the generator. It will
				 * simply return nullptr for any other types, even though a generated object will be spawned!
				 * The generator function is towards the end of OnStarting().
				 */
				auto pProjectile = pPool->NewObject<Projectile>(std::move(name));
				if (pProjectile)
				{
					if (const auto pRocket = FindObject<Rocket>(m_hRocket))
					{
						const auto& t = pRocket->m_transform;
						pProjectile->m_transform.SetOrientation(t.WorldOrientation());
						// Move it a bit (75 units) in the direction of the orientation
						const auto pos = t.WorldPosition() + (t.WorldOrientation() * 75);
						pProjectile->m_transform.SetPosition(pos);
						// Calling SetEnabled(true) after performing all operations on a GameObject
						// updates all its components (Collision, Primitive, etc) to the present state;
						// this is especially useful for pooled objects that DON'T have `OnCreate()` called again!
						pProjectile->SetEnabled(true);
					}
				}
			}
		}
		return false;
	};
	// Pass the functor object
	RegisterInput(std::move(onInput));

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
	}
	// Note: Press [Shift] + [c] to toggle collider debug shapes

// This is guarded because Collider::s_bShowDebugShape doesn't compile on non DEBUGGING builds
#if defined(DEBUGGING)
	// Add a UIButton, that derives from UIWidget, which basically models an interactable UI element,
	// and providing an m_onInteract Delegate for derived classes to call, and a public API to register
	// callbacks via. UIWidget calls Select()/Deselect() on mouse hovers, and InteractBegin()/InteractEnd()
	// on left mouse button (KeyType::MOUSE_BTN_0) down/up. UIButton simply fires m_onInteract() on InteractEnd().
	auto hColliderBtn = NewObject<UIButton>("ColliderButton");
	if (auto pBtn = FindObject<UIButton>(hColliderBtn))
	{
		pBtn->m_layer = Layers::L1500_UI;
		// Note: Call SetUIText on all UIObjects (uses another GameObject instead of itself)
		pBtn->SetUIText("Toggle Colliders").m_transform.SetPosition(g_pGFX->WorldProjection({0, -1}) + Vector2(0, 100));
		// Short lambdas can fit it one line, even with a token push_back.
		// This is basically what `RegisterInput()` also does: stores a token in a vector which is cleared on `Stop()`.
		// The "token" is used by the underlying Delegate to determine if the calling object is still alive;
		// discarded tokens are considered to represent "dead" objects and will be removed from its callback list.
		m_miscTokens.push_back(pBtn->Register([]() { Collider::s_bShowDebugShape = !Collider::s_bShowDebugShape; }));
		m_uiButtons.push_back(hColliderBtn);
	}
#endif
	// Add a button to load the previous world
	auto hPrevButton = NewObject<UIButton>("PreviousButton");
	if (auto pBtn = FindObject<UIButton>(hPrevButton))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText("<<").m_transform.SetPosition(g_pGFX->WorldProjection({-1, 1}) + Vector2(100, -100));
		m_miscTokens.push_back(pBtn->Register([]() { g_pContext->LoadWorld(PREV_WORLD); }));
		m_uiButtons.push_back(hPrevButton);
	}

	// Add a button to load the next world
	auto hNextButton = NewObject<UIButton>("NextButton");
	if (auto pBtn = FindObject<UIButton>(hNextButton))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText(">>").m_transform.SetPosition(g_pGFX->WorldProjection({1, 1}) + Vector2(-100, -100));
		m_miscTokens.push_back(pBtn->Register([]() { g_pContext->LoadWorld(NEXT_WORLD); }));
		m_uiButtons.push_back(hNextButton);
	}
	// This is an Object Pool, which is what would be the right way to spawn
	// new bubbles in Tutorial3. The way to use it is to first set up the "generator":
	// a function that instantiates a new GameObject (or derived type) and returns a pointer
	// to it. The pool will use this to create new objects (if none are available).
	m_bubbleCount = 0;
	m_hBubbles = NewPool("Bubbles");
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		auto bubblesGen = [this]() -> GameObject* {
			// We want to set up everything related to Bubbles except their transform, which will be done
			// per instance obtained from the pool at runtime (not this code, the one in Tick).
			auto handle = NewObject<Bubble>("Bubble");
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
				pBubble->m_layer = Layers::L0100_DEFAULT - 10;
				return pBubble;
			}
			// Lambda requires a GameObject* return, so return null here
			return nullptr;
		};
		pBubbles->SetGenerator(std::move(bubblesGen));
		// PreInstantiate will instantiate the number of objects passed, using the given string as a
		// name prefix. This prevents the need to have to instantiate new objects later (no allocations
		// during hot game-time is extremely desriable, though not always achievable).
		pBubbles->PreInstantiate("Bubble", INIT_BUBBLES);
	}

	// Same thing for projectiles
	m_projectileCount = 0;
	m_hProjectiles = NewPool("Projectiles");
	if (auto pProjectiles = FindPool(m_hProjectiles))
	{
		auto projectileGen = [this]() -> GameObject* {
			auto handle = NewObject<Projectile>("Projectile");
			if (auto pProjectile = FindObject<Projectile>(handle))
			{
				pProjectile->Instantiate(Primitive::Type::Rectangle);
				ShapeData data;
				data.oSize = {75, 6};
				data.oFill = Colour::Cyan;
				pProjectile->SetShape(data).m_layer = Layers::L0100_DEFAULT + 10;
				// Add an AABB collider to this Collision component (with a callback that just calls Projectile::OnHit,
				// which is coded to destroy the colliding object if it is a `Bubble`)
				auto onCollision = [this, handle](Collision::Info info) {
					if (auto pProjectile = FindObject<Projectile>(handle))
					{
						pProjectile->OnHit(info);
					}
				};
				auto token = pProjectile->GetCollision().AddAABB(std::move(onCollision), AABBData({20, 20}));
				m_miscTokens.push_back(token);
				if (auto pRocket = FindObject<Rocket>(m_hRocket))
				{
					// Setup the speed here itself, since we want all instances to share that property
					pProjectile->m_speed = pRocket->m_speed * 5;
				}
				return pProjectile;
			}
			return nullptr;
		};
		pProjectiles->SetGenerator(std::move(projectileGen));
		pProjectiles->PreInstantiate("Projectile", 10);
	}

	// Tilemap
	m_hTilemap = NewObject<Tilemap>("TestTiles");
	if (auto pTilemap = FindObject<Tilemap>(m_hTilemap))
	{
		if (auto pTexture = g_pResources->Find<Texture>(m_hTile))
		{
			pTilemap->FillSpace(g_pGFX->WorldSpace(), *pTexture);
		}
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
		if (pBubbles->Spawned() < INIT_BUBBLES)
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
	m_hBubbles = m_hProjectiles = m_hRocket = m_hMainText = m_hTilemap = INVALID_HANDLE;
	m_miscTokens.clear();
	m_uiButtons.clear();
}
} // namespace ME
