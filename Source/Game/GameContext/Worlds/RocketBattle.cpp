#include <array>
#include <sstream>
#include "GameLoop/GameLoop.h"
#include "Engine/GameServices.h"
#include "Engine/Physics/ColliderData.h"
#include "../ObjectPool.h"
#include "../Objects/Tilemap.h"
#include "../Objects/Bubble.h"
#include "../Objects/Projectile.h"
#include "../Objects/Rocket.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/UI/UICheckbox.h"
#include "RocketBattle.h"

namespace ME
{
namespace
{
static const std::string CURRENT_WORLD = "RocketBattle";
static const std::string PREV_WORLD = "Intro";
constexpr u32 INIT_BUBBLES = 5;

void LoadPreviousWorld()
{
	bool bPrevWorldExists = g_pContext->LoadWorld(PREV_WORLD);
	if (!bPrevWorldExists)
	{
		LOG_I("[%s] Previous world not found, exiting game", PREV_WORLD.data());
		GameLoop::Stop();
	}
}
} // namespace

const RocketBattleWorld::Timers RocketBattleWorld::s_defaultTimer;

void RocketBattleWorld::OnCreate()
{
	g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	g_pResources->Load<Texture>("Textures/Exhaust_128x128_2x4.png");
	m_hTile = g_pResources->Load<Texture>("Textures/PurpleBrickTile_512x512.png");
}

void RocketBattleWorld::OnStarting()
{
	m_gameState = GameState::Playing;
	m_playedTime = Time::Zero;
	m_bubblesToSpawn = INIT_BUBBLES;
	m_delta = s_defaultTimer;

	auto onInput = [this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Escape))
		{
			LoadPreviousWorld();
		}
		else if (m_gameState == GameState::Playing)
		{
			if (frame.IsReleased(KeyType::MOUSE_BTN_0) || frame.IsReleased(KeyCode::LControl))
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
		}
		else if (m_gameState == GameState::RocketDestroyed)
		{
			if (frame.IsReleased(KeyCode::Space))
			{
				g_pContext->LoadWorld(CURRENT_WORLD);
			}
		}
		return false;
	};
	RegisterInput(std::move(onInput));

	m_hLeftTop = NewObject<GameObject>("LeftTop");
	m_hMidTop = NewObject<GameObject>("MidTop");
	m_hRightTop = NewObject<GameObject>("RightTop");
	m_hMidCentre0 = NewObject<GameObject>("MidCentre0");
	m_hMidCentre1 = NewObject<GameObject>("MidCentre1");
	auto instantiateText = [&](const HObj& hObj, const Vector2& nPos, const Vector2& offset, u32 size = 24) -> GameObject* {
		if (auto pObj = FindObject<GameObject>(hObj))
		{
			pObj->SetEnabled(true);
			pObj->Instantiate(Primitive::Type::Text);
			TextData data;
			data.oCharSize = size;
			pObj->SetText(data);
			pObj->m_transform.SetPosition(g_pGFX->WorldProjection(nPos) + offset);
			return pObj;
		}
		return nullptr;
	};
	instantiateText(m_hLeftTop, {-1, 1}, {100, -100});
	auto pMidTop = instantiateText(m_hMidTop, {0, 1}, {0, -100}, 50);
	instantiateText(m_hRightTop, {1, 1}, {-100, -100});
	auto pMidCentre0 = instantiateText(m_hMidCentre0, {0, 0}, {0, 100}, 65);
	auto pMidCentre1 = instantiateText(m_hMidCentre1, {0, 0}, {0, -100});
	if (pMidTop)
	{
		TextData data;
		data.oFill = Colour::Yellow;
		pMidTop->SetText(data);
	}
	if (pMidCentre0)
	{
		TextData data;
		data.oFill = Colour(250, 30, 30);
		pMidCentre0->SetText(data);
	}
	if (pMidCentre1)
	{
		TextData data;
		data.oFill = Colour::Cyan;
		data.oText = "\n [  Tab  ] Chase/Rotate"
					 "\n [ Mouse ] Target"
					 "\n [  MB0  ] Fire";
		pMidCentre1->SetText(data);
	}
	SetPlayerStats();
	m_hRocket = NewObject<Rocket>("Rocket");
	if (auto pRocket = FindObject<Rocket>(m_hRocket))
	{
		pRocket->m_speed = 2;
	}

#if defined(DEBUGGING)
	m_hColliderBtn = NewObject<UIButton>("ColliderButton");
	if (auto pBtn = FindObject<UIButton>(m_hColliderBtn))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText("Toggle Colliders").m_transform.SetPosition(g_pGFX->WorldProjection({0, -1}) + Vector2(0, 100));
		pBtn->SetEnabled(true);
		m_miscTokens.push_back(pBtn->Register([]() { Collider::s_bShowDebugShape = !Collider::s_bShowDebugShape; }));
		m_uiButtons.push_back(m_hColliderBtn);
	}
#endif

	m_hRestartBtn = NewObject<UIButton>("RestartButton");
	if (auto pBtn = FindObject<UIButton>(m_hRestartBtn))
	{
		pBtn->m_layer = Layers::L1500_UI;
		pBtn->SetUIText("Restart Level").m_transform.SetPosition(g_pGFX->WorldProjection({0, -1}) + Vector2(0, 100));
		pBtn->SetEnabled(false);
		m_miscTokens.push_back(pBtn->Register([]() { g_pContext->LoadWorld(CURRENT_WORLD); }));
		m_uiButtons.push_back(m_hRestartBtn);
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
				auto onCollision = [this, handle](Collision::Info info) {
					if (auto pBubble = FindObject<Bubble>(handle))
					{
						if (dynamic_cast<Rocket*>(info.pInstigator))
						{
							OnRocketDestruction();
						}
					}
				};
				auto token = pBubble->GetCollision().AddCircle(onCollision, pBubble->m_diameter);
				m_miscTokens.push_back(token);
				pBubble->m_layer = Layers::L0100_DEFAULT;
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

void RocketBattleWorld::Tick(Time dt)
{
	m_playedTime += dt;
	m_delta.bubbleSpawn -= dt;
	m_delta.bubbleTTL -= dt;

	if (m_delta.bubbleSpawn <= Time::Zero)
	{
		++m_bubblesToSpawn;
		m_delta.bubbleSpawn = s_defaultTimer.bubbleSpawn;
	}

	if (auto pRocket = FindObject<Chaser>(m_hRocket))
	{
		SpriteData data;
		data.oFill = pRocket->m_state == Chaser::State::Chasing ? Colour(243, 31, 65) : Colour::White;
		pRocket->SetSprite(data);
	}
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		if (pBubbles->Spawned() < m_bubblesToSpawn && m_gameState == GameState::Playing)
		{
			std::string name = "Bubble_";
			name += std::to_string(m_bubbleCount++);
			auto pBubble = pBubbles->NewObject<Bubble>(std::move(name));
			const Fixed nX = Maths::Random::Range(-Fixed::One, Fixed::One);
			const Fixed nY = -Fixed(0.8f);
			pBubble->m_transform.SetPosition(g_pGFX->WorldProjection({nX, nY}));
			pBubble->SetEnabled(true);

			if (m_delta.bubbleTTL <= Time::Zero)
			{
				pBubble->NextTTL();
				m_delta.bubbleTTL = s_defaultTimer.bubbleTTL;
			}
		}
	}

	if (m_gameState == GameState::Playing)
	{
		if (m_projectileCount > 0)
		{
			m_accuracy = std::round(((f64(m_projectileHitCount) / f64(m_projectileCount)) * 100) * 100) / 100;
		}
		SetPlayerStats();
	}

	GameWorld::Tick(dt);
}

void RocketBattleWorld::OnRocketDestruction()
{
	m_gameState = GameState::RocketDestroyed;
	if (auto pRocket = FindObject<Rocket>(m_hRocket))
	{
		pRocket->Destroy();
	}
	if (auto pBubbles = FindPool(m_hBubbles))
	{
		pBubbles->DestroyAll();
	}
	if (auto pProjectiles = FindPool(m_hProjectiles))
	{
		pProjectiles->DestroyAll();
	}
	if (auto pMidCentre0 = FindObject<GameObject>(m_hMidCentre0))
	{
		pMidCentre0->SetText("GAME OVER");
	}
	if (auto pMidCentre1 = FindObject<GameObject>(m_hMidCentre1))
	{
		pMidCentre1->SetText("[ Space ] Restart\n[  Esc  ] Exit");
	}
	if (auto pColliderBtn = FindObject<UIButton>(m_hColliderBtn))
	{
		pColliderBtn->SetEnabled(false);
	}
	if (auto pRestartBtn = FindObject<UIButton>(m_hRestartBtn))
	{
		pRestartBtn->SetEnabled(true);
	}
	if (auto pTilemap = FindObject<Tilemap>(m_hTilemap))
	{
		pTilemap->SetEnabled(false);
	}
}

void RocketBattleWorld::OnStopping()
{
	m_hBubbles = m_hProjectiles = m_hRocket = m_hTilemap = m_hColliderBtn = m_hRestartBtn = INVALID_HANDLE;
	m_hLeftTop = m_hMidTop = m_hRightTop = m_hMidCentre0 = m_hMidCentre1 = INVALID_HANDLE;
	m_playerScore = m_projectileHitCount = 0;
	m_accuracy = 0.0f;
	m_miscTokens.clear();
	m_uiButtons.clear();
}

void RocketBattleWorld::SetPlayerStats()
{
	static std::array<char, 8> s_buf;
	if (auto pLeftTop = FindObject<GameObject>(m_hLeftTop))
	{
		std::snprintf(s_buf.data(), s_buf.size(), "%3.2f", m_accuracy);
		std::stringstream text;
		text << m_projectileHitCount << "/" << m_projectileCount << "\n" << s_buf.data() << "%";
		pLeftTop->SetText(text.str());
	}
	if (auto pMidTop = FindObject<GameObject>(m_hMidTop))
	{
		pMidTop->SetText(std::to_string(m_playerScore));
	}
	if (auto pRightTop = FindObject<GameObject>(m_hRightTop))
	{
		std::snprintf(s_buf.data(), s_buf.size(), "%us", (u32)m_playedTime.AsSeconds());
		pRightTop->SetText(std::string(s_buf.data()));
	}
}
} // namespace ME
