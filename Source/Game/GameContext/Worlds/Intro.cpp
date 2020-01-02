#include "Intro.h"
#include "Engine/GameServices.h"
#include "GameLoop/GameLoop.h"

namespace ME
{
void IntroWorld::OnStarting()
{
	auto hTitle = NewObject<GameObject>("Title");
	if (auto pText = FindObject<GameObject>(hTitle))
	{
		pText->Instantiate(Primitive::Type::Text);
		TextData data;
		data.oText = "ROCKET BATTLE";
		data.oCharSize = 60;
		pText->SetText(data);
		pText->m_transform.SetPosition({0, 100});
	}

	m_hSubtitle = NewObject<GameObject>("Subtitle");
	if (auto pText = FindObject<GameObject>(m_hSubtitle))
	{
		pText->Instantiate(Primitive::Type::Text);
		TextData data;
		data.oText = "... Press any key to begin ...";
		data.oCharSize = 30;
		pText->SetText(data);
		pText->m_transform.SetPosition({0, -200});
	}
	{
		TextData data;
		data.oCharSize = 20;
		m_hLeftText = NewObject<GameObject>("LeftBottom");
		Fixed dy = 50;
		Fixed dx = 100;
		if (auto pLeftBottom = FindObject<GameObject>(m_hLeftText))
		{
			pLeftBottom->Instantiate(Primitive::Type::Text);
			data.oText = "Karnage";
			data.oFill = Colour(150, 10, 30);
			pLeftBottom->SetText(data);
			pLeftBottom->m_transform.SetPosition(g_pGFX->WorldProjection({-1, -1}) + Vector2(dx, dy));
			pLeftBottom->SetEnabled(false);
		}
		m_hRightText = NewObject<GameObject>("RightBottom");
		if (auto pRightBottom = FindObject<GameObject>(m_hRightText))
		{
			pRightBottom->Instantiate(Primitive::Type::Text);
			data.oText = "KingHarley";
			data.oFill = Colour(150, 20, 150);
			pRightBottom->SetText(data);
			pRightBottom->m_transform.SetPosition(g_pGFX->WorldProjection({1, -1}) + Vector2(-dx, dy));
			pRightBottom->SetEnabled(false);
		}
	}
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Escape))
		{
			GameLoop::Stop();
		}
		else if (!frame.released.empty() && !frame.IsPressed(KeyCode::Escape) && !frame.IsHeld(KeyCode::Escape))
		{
			g_pContext->LoadWorld("RocketBattle");
		}
		return false;
	});
}

void IntroWorld::Tick(Time dt)
{
	m_elapsed += dt;
	if (auto pText = FindObject<GameObject>(m_hSubtitle))
	{
		// 2.0 to double the frequency per frame, .3 to displace the phase from when credits show up
		Fixed a = Maths::Abs(Fixed(m_elapsed.AsSeconds() * 2.3f).Cos());
		UByte alpha((u8)(a * 255).ToU32());
		TextData data;
		data.oFill = {255, 255, 255, alpha};
		pText->SetText(data);
	}
	if (!m_bCreditsShown && m_elapsed >= Time::Seconds(1.0f))
	{
		m_bCreditsShown = true;
		auto& world = GameWorld::Active();
		auto pLeft = world.FindObject<GameObject>(m_hLeftText);
		auto pRight = world.FindObject<GameObject>(m_hRightText);
		if (pLeft && pRight)
		{
			pLeft->SetEnabled(true);
			pRight->SetEnabled(true);
		}
	}
	GameWorld::Tick(dt);
}

void IntroWorld::OnStopping()
{
	m_elapsed = Time::Zero;
	m_hSubtitle = INVALID_HANDLE;
	m_bCreditsShown = false;
}
} // namespace ME
