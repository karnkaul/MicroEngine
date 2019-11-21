#include <array>
#include "Engine/GameServices.h"
#include "Engine/Physics/ColliderData.h"
#include "../../ObjectPool.h"
#include "../../Objects/Tilemap.h"
#include "../../Objects/Tutorials/Bubble.h"
#include "../../Objects/Tutorials/Projectile.h"
#include "../../Objects/Tutorials/Rocket.h"
#include "../../Objects/UI/UIButton.h"
#include "Tutorial6.h"

namespace ME
{
namespace
{
static const std::string NEXT_WORLD = "Temp";
static const std::string PREV_WORLD = "Tutorial5";
constexpr u32 INIT_BUBBLES = 5;
} // namespace

void Tutorial6::OnCreate()
{
	g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	g_pResources->Load<Texture>("Textures/Exhaust_128x128_2x4.png");
	m_hTile = g_pResources->Load<Texture>("Textures/PurpleBrickTile_512x512.png");
}

void Tutorial6::OnStarting()
{
	m_playedTime = Time::Zero;

	auto onInput = [this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld(NEXT_WORLD);
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld(PREV_WORLD);
		}
		else if (frame.IsReleased(KeyType::MOUSE_BTN_0) || frame.IsReleased(KeyCode::LControl))
		{
			std::string name = "Projectile_";
			name += std::to_string(m_projectileCount++);
			if (auto pPool = FindPool(m_hProjectiles))
			{
				auto pProjectile = pPool->NewObject<Projectile>(std::move(name));
				if (pProjectile)
				{
					if (const auto pRocket = FindObject<Rocket>(m_hRocket))
					{
						const auto& t = pRocket->m_transform;
						pProjectile->m_transform.SetOrientation(t.WorldOrientation());
						const auto pos = t.WorldPosition() + (t.WorldOrientation() * 75);
						pProjectile->m_transform.SetPosition(pos);
						pProjectile->SetEnabled(true);
						--m_playerScore;
					}
				}
			}
		}
		return false;
	};
	RegisterInput(std::move(onInput));

	m_hMainText = NewObject<GameObject>("MainText");
	if (auto pText = FindObject<GameObject>(m_hMainText))
	{
		pText->Instantiate(Primitive::Type::Text);
		pText->SetText("Press [Tab] to toggle chasing");
	}
	m_hPlayerStatistics = NewObject<GameObject>("PlayerStatistics");
	if (auto pPlayerStatistics = FindObject<GameObject>(m_hPlayerStatistics))
	{
		pPlayerStatistics->Instantiate(Primitive::Type::Text);
		std::string text = "Player Score: ";
		text += std::to_string(m_playerScore);
		text += "\nAccuracy: ";
		text += std::to_string(m_projectileHitCount);
		text += "/";
		text += std::to_string(m_projectileCount);

		pPlayerStatistics->SetText(std::move(text));
		pPlayerStatistics->m_transform.SetPosition(g_pGFX->WorldProjection({Fixed(0.0f), Fixed(0.6f)}));
	}
	m_hRocket = NewObject<Rocket>("Rocket");
	if (auto pRocket = FindObject<Rocket>(m_hRocket))
	{
		pRocket->m_speed = 2;
	}

#if defined(DEBUGGING)
	auto hColliderBtn = NewObject<UIButton>("ColliderButton");
	if (auto pBtn = FindObject<UIButton>(hColliderBtn))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText("Toggle Colliders").m_transform.SetPosition(g_pGFX->WorldProjection({0, -1}) + Vector2(0, 100));
		m_miscTokens.push_back(pBtn->Register([]() { Collider::s_bShowDebugShape = !Collider::s_bShowDebugShape; }));
		m_uiButtons.push_back(hColliderBtn);
	}
#endif
	auto hPrevButton = NewObject<UIButton>("PreviousButton");
	if (auto pBtn = FindObject<UIButton>(hPrevButton))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText("<<").m_transform.SetPosition(g_pGFX->WorldProjection({-1, 1}) + Vector2(100, -100));
		m_miscTokens.push_back(pBtn->Register([]() { g_pContext->LoadWorld(PREV_WORLD); }));
		m_uiButtons.push_back(hPrevButton);
	}

	auto hNextButton = NewObject<UIButton>("NextButton");
	if (auto pBtn = FindObject<UIButton>(hNextButton))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText(">>").m_transform.SetPosition(g_pGFX->WorldProjection({1, 1}) + Vector2(-100, -100));
		m_miscTokens.push_back(pBtn->Register([]() { g_pContext->LoadWorld(NEXT_WORLD); }));
		m_uiButtons.push_back(hNextButton);
	}

	m_bubbleCount = 0;
	m_hBubbles = NewPool("Bubbles");
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		auto bubblesGen = [this]() -> GameObject* {
			auto handle = NewObject<Bubble>("Bubble");
			if (auto pBubble = FindObject<Bubble>(handle))
			{
				const s32 ttlSecs = Maths::Random::Range(8, 10);
				pBubble->m_ttl = Time::Seconds(static_cast<f32>(ttlSecs));
				const Fixed nSize = Fixed(ttlSecs) / Fixed(10);
				pBubble->m_diameter += (nSize * 20);
				pBubble->m_ySpeed -= (nSize * Fixed::OneHalf);
				pBubble->GetCollision().AddCircle(nullptr, pBubble->m_diameter);
				pBubble->m_layer = Layers::L0100_DEFAULT - 10;
				return pBubble;
			}
			return nullptr;
		};
		pBubbles->SetGenerator(std::move(bubblesGen));
		pBubbles->PreInstantiate("Bubble", INIT_BUBBLES);
	}

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
				auto onCollision = [this, handle](Collision::Info info) {
					if (auto pProjectile = FindObject<Projectile>(handle))
					{
						if (pProjectile->OnHit(info))
						{
							m_playerScore += 2;
							++m_projectileHitCount;
						}
					}
				};
				auto token = pProjectile->GetCollision().AddAABB(std::move(onCollision), AABBData({20, 20}));
				m_miscTokens.push_back(token);
				if (auto pRocket = FindObject<Rocket>(m_hRocket))
				{
					pProjectile->m_speed = pRocket->m_speed * 5;
				}
				return pProjectile;
			}
			return nullptr;
		};
		pProjectiles->SetGenerator(std::move(projectileGen));
		pProjectiles->PreInstantiate("Projectile", 10);
	}

	m_hTilemap = NewObject<Tilemap>("TestTiles");
	if (auto pTilemap = FindObject<Tilemap>(m_hTilemap))
	{
		if (auto pTexture = g_pResources->Find<Texture>(m_hTile))
		{
			pTilemap->FillSpace(g_pGFX->WorldSpace(), *pTexture);
		}
	}
}

void Tutorial6::Tick(Time dt)
{
	m_playedTime += dt;

	if (auto pRocket = FindObject<Chaser>(m_hRocket))
	{
		SpriteData data;
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
			const Fixed nX = Maths::Random::Range(-Fixed::One, Fixed::One);
			const Fixed nY = -Fixed(0.8f);
			pBubble->m_transform.SetPosition(g_pGFX->WorldProjection({nX, nY}));
			pBubble->SetEnabled(true);
		}
	}

	/*if (auto pProjectiles = FindPool(m_hProjectiles))
	{
		for (auto pProjectile : pProjectiles->m_objects)
		{
			
		}
	}*/

	if (auto pPlayerStatistics = FindObject<GameObject>(m_hPlayerStatistics))
	{
		/*std::string text = "Player Score: ";
		text += std::to_string(m_playerScore);
		pPlayerStatistics->SetText(std::move(text));*/
		if (m_projectileCount > 0)
		{
			m_accuracy = std::round(((f64(m_projectileHitCount) / f64(m_projectileCount)) * 100) * 100) / 100;
		}

		std::array<char, 7> buf;
		std::snprintf(buf.data(), buf.size(), "%3.2f", m_accuracy);
		//SPRINTF(buf.data(), buf.size(), "%3.2f", m_accuracy);
		std::string text = "Player Score: ";
		text += std::to_string(m_playerScore);
		text += "\nAccuracy: ";
		text += std::to_string(m_projectileHitCount);
		text += "/";
		text += std::to_string(m_projectileCount);
		text += " (";
		text += buf.data();
		text += "%)\nTime Played: ";
		buf.fill('0');
		std::snprintf(buf.data(), buf.size(), "%.0f", m_playedTime.AsSeconds());
		text += buf.data();
		text += " seconds\n";
		pPlayerStatistics->SetText(std::move(text));
	}

	GameWorld::Tick(dt);
}

void Tutorial6::OnStopping()
{
	m_hBubbles = m_hProjectiles = m_hRocket = m_hMainText = m_hTilemap = m_hPlayerStatistics = INVALID_HANDLE;
	m_playerScore = m_projectileHitCount = 0;
	m_miscTokens.clear();
	m_uiButtons.clear();
}
} // namespace ME
