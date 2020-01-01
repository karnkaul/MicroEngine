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
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Escape))
		{
			GameLoop::Stop();
		}
		else if (frame.HasData() && !frame.IsPressed(KeyCode::Escape) && !frame.IsHeld(KeyCode::Escape))
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
		Fixed a = Fixed(m_elapsed.AsSeconds() * 2).Cos();
		UByte alpha((u8)(a * 255).ToU32());
		TextData data;
		data.oFill = {255, 255, 255, alpha};
		pText->SetText(data);
	}
	GameWorld::Tick(dt);
}

void IntroWorld::OnStopping()
{
	m_elapsed = Time::Zero;
	m_hSubtitle = INVALID_HANDLE;
}
} // namespace ME
