#include "TutorialKing.h"
#include "Engine/GameServices.h"
#include "../../Objects/Tutorials/Controller.h"

namespace ME
{
void TutorialKing::OnCreated()
{
	g_pResources->Load<Text>("Tutorials/T4_Controller.json");
}

void TutorialKing::OnStarting()
{
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Tutorial1"); // Have to
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial0");
		}
		return false;
	});

	m_hMainText = NewObject<GameObject>("MainText");
	auto pIntroText = FindObject<GameObject>(m_hMainText);
	if (pIntroText)
	{
		pIntroText->Instantiate(Primitive::Type::Text);
		pIntroText->SetText("This is KingHarley's Tutorial!");
	}

	m_hPlayer = NewObject<Controller>("Player");
	// auto pPlayer = FindObject<Controller>(m_hPlayer);
}

void TutorialKing::OnStopping()
{
	m_hMainText = INVALID_HANDLE;
	m_hPlayer = INVALID_HANDLE;
}
} // namespace ME
