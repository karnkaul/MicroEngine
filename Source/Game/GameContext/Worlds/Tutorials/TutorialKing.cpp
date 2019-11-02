#include "TutorialKing.h"
#include "Engine/GameServices.h"
#include "../../Objects/Tutorials/Controller.h"
#include "../../Objects/Tutorials/Food.h"

namespace ME
{
void TutorialKing::OnCreate()
{
	g_pResources->Load<Text>("Tutorials/T4_Controller.json");
}

void TutorialKing::OnStarting()
{
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Tutorial1");
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial0");
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
		data.oSize = {50, 50};
		data.oFill = Colour::Blue;
		data.oOutline = Colour::Yellow;
		data.oBorder = 2;
		pPlayer->SetShape(data);
		pPlayer->m_layer = playerLayer;
		pPlayer->m_speed = Fixed(0.5f);
		pPlayer->GetCollision().AddCircle(nullptr, 100);
	}

	m_hFood = NewObject<Food>("Food");
	auto pFood = FindObject<Food>(m_hFood);
	auto handle = m_hFood;
	if (pFood)
	{
		pFood->Instantiate(Primitive::Type::Circle);
		data.oFill = Colour::Red;
		data.oSize = {25, 25};
		pFood->SetShape(data);
		pFood->m_layer = playerLayer;
		pFood->m_transform.SetPosition(Vector2(150, 150));

		auto OnHit = [this, handle](Collision::Info info) {
			if (auto pFood = FindObject<Food>(handle))
			{
				pFood->OnHit(info);
			}
		};
		auto token = pFood->GetCollision().AddCircle(std::move(OnHit), 50);
		m_callbackTokens.push_back(token);
	}
}

void TutorialKing::OnStopping()
{
	m_hMainText = INVALID_HANDLE;
	m_hPlayer = INVALID_HANDLE;
	m_hFood = INVALID_HANDLE;
}
} // namespace ME
