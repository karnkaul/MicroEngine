#include "TutorialKing.h"
#include "Engine/GameServices.h"
#include "../../Objects/Controller.h"
#include "../../Objects/Tutorials/Food.h"

namespace ME
{
namespace
{
static const std::string nextWorld = "Tutorial6";
static const std::string previousWorld = "Tutorial0";
static const std::string currentWorld = "TutorialKing";
constexpr u32 INIT_FOODS = 5;
} // namespace

void TutorialKing::OnCreate()
{
	g_pResources->Load<Text>("Tutorials/T4_Controller.json");
}

void TutorialKing::OnStarting()
{
	m_gameState = GameState::PLAYING;

	RegisterInput([this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld(nextWorld);
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld(previousWorld);
		}
		else if (frame.IsReleased(KeyCode::R) && m_gameState == GameState::WINNING)
		{
			g_pContext->LoadWorld(currentWorld);
		}

		return false;
	});

	m_hMainText = NewObject<GameObject>("MainText");
	auto pIntroText = FindObject<GameObject>(m_hMainText);
	LayerID playerLayer = 0;
	if (pIntroText)
	{
		pIntroText->Instantiate(Primitive::Type::Text);
		pIntroText->SetText("This is KingHarley's Tutorial!");
		pIntroText->m_transform.SetPosition(Vector2(0, 300));
		playerLayer = pIntroText->m_layer - 1;
	}

	m_hPlayer = NewObject<Controller>("Player");
	auto pPlayer = FindObject<Controller>(m_hPlayer);
	ShapeData data;
	if (pPlayer)
	{
		pPlayer->Instantiate(Primitive::Type::Circle);
		data.oSize = {pPlayer->m_radius, pPlayer->m_radius};
		data.oFill = Colour::Blue;
		data.oOutline = Colour::Yellow;
		data.oBorder = 2;
		pPlayer->SetShape(data);
		pPlayer->m_layer = playerLayer;
		pPlayer->m_speed = Fixed(0.5f);
		pPlayer->GetCollision().AddCircle(nullptr, 100);
	}

	m_foodCount = 0;
	m_hFood = NewPool("Food");
	if (auto pFoods = FindPool(m_hFood))
	{
		auto foodGen = [this, playerLayer]() -> GameObject* {
			auto handle = NewObject<Food>("Food");
			if (auto pFood = FindObject<Food>(handle))
			{
				ShapeData data;
				data.oFill = Colour::Red;
				data.oSize = {25, 25};
				pFood->SetShape(data);
				pFood->m_layer = playerLayer;

				auto OnHit = [this, handle](Collision::Info info) {
					if (auto pFood = FindObject<Food>(handle))
					{
						pFood->OnHit(info);
					}
				};
				auto token = pFood->GetCollision().AddCircle(std::move(OnHit), 50);
				m_callbackTokens.push_back(token);

				return pFood;
			}
			return nullptr;
		};

		pFoods->SetGenerator(std::move(foodGen));
		pFoods->PreInstantiate("Food", INIT_FOODS);
	}
}

void TutorialKing::Tick(Time dt)
{
	if (auto pFoods = FindPool(m_hFood))
	{
		if (pFoods->Spawned() < INIT_FOODS && m_gameState == GameState::PLAYING)
		{
			std::string name = "Food_";
			name += std::to_string(m_foodCount++);
			auto pFood = pFoods->NewObject(std::move(name));
			const Fixed nX = Maths::Random::Range(Fixed(-0.8f), Fixed(0.8f));
			const Fixed nY = Maths::Random::Range(Fixed(-0.8f), Fixed(0.8f));
			pFood->m_transform.SetPosition(g_pGFX->WorldProjection({nX, nY}));
			pFood->SetEnabled(true);
		}
	}

	if (auto pPlayer = FindObject<Controller>(m_hPlayer))
	{
		if (pPlayer->m_radius > Fixed(90))
		{
			OnWin();
		}
	}

	GameWorld::Tick(dt);
}

void TutorialKing::OnWin()
{
	m_gameState = GameState::WINNING;
	if (auto pPlayer = FindObject<Controller>(m_hPlayer))
	{
		pPlayer->Destroy();
	}

	if (auto pFoods = FindPool(m_hFood))
	{
		pFoods->DestroyAll();
	}

	if (auto pMainText = FindObject<GameObject>(m_hMainText))
	{
		pMainText->SetText("You have WON!\n[Space] To load next world\n[ESC] To load previous world\n[R] To reload current world");
		pMainText->m_transform.SetPosition(Vector2(0, 0));
	}
}

void TutorialKing::OnStopping()
{
	m_hMainText = INVALID_HANDLE;
	m_hPlayer = INVALID_HANDLE;
	m_hFood = INVALID_HANDLE;
	m_callbackTokens.clear();
}
} // namespace ME
