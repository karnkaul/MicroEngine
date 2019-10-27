#include "Engine/GameServices.h"
#include "../../Objects/Tutorials/Controller.h"
#include "Tutorial4.h"

namespace ME
{
void Tutorial4::OnCreate()
{
	// A new type of Resource: Text, can be used to load config etc.
	// This Resource will be used by Controller, so the path is a tight coupling
	// to the world. It can be loosened by each world having a manifest file
	// using which it loads assets; but that's for when we are in Production.
	g_pResources->Load<Text>("Tutorials/T4_Controller.json");
}

void Tutorial4::OnStarting()
{
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Tutorial5");
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial3");
		}
		return false;
	});

	m_hController = NewObject<Controller>("Controller");
	LayerID controllerLayer = 0;
	if (auto pController = FindObject<Controller>(m_hController))
	{
		// The first instance of split configuration: set up the appearance of
		// the object here, which Controller's logic doesn't care about.
		pController->Instantiate(Primitive::Type::Circle);
		ShapeData data;
		data.oSize = {50, 50};
		data.oFill = Colour::Magenta;
		data.oOutline = Colour::Cyan;
		data.oBorder = 2;
		pController->SetShape(data);
		controllerLayer = pController->m_layer;
	}

	m_hMainText = NewObject<GameObject>("MainText");
	if (auto pMainText = FindObject<GameObject>(m_hMainText))
	{
		pMainText->Instantiate(Primitive::Type::Text);
		pMainText->SetText("Pres [WASD] / [Up/Down/Left/Right] to move\nHold [Backspace] to grind game to a halt");
		pMainText->m_layer = controllerLayer + 1;
	}
}

void Tutorial4::OnStopping()
{
	// One-liner, requires addresses because temporary vectors/initializer_lists cannot be modified
	DestroyAll({&m_hController, &m_hMainText});
}
} // namespace ME
