#include "Engine/GameServices.h"
#include "Tutorial1.h"

namespace ME
{
void Tutorial1::OnCreated()
{
	Assert(g_pResources, "Resources is null!");
	// Get a handle to the Default-Serif.ttf font; load heavy stuff when created
	m_hSerifFont = g_pResources->Load<Font>("Default-Serif.ttf");
}

void Tutorial1::OnStarting()
{
	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			if (frame.IsReleased(KeyCode::Space))
			{
				const std::string nextWorldID = "Tutorial2";
				if (!g_pContext->LoadWorld(nextWorldID))
				{
					LOG_W("[%s] %s GameWorld does not exist!", m_name.data(), nextWorldID.data());
				}
			}
			// Go back to Tutorial0 on Escape
			if (frame.IsReleased(KeyCode::Escape))
			{
				g_pContext->LoadWorld("Tutorial0");
			}
			if (frame.IsReleased(KeyCode::Tab))
			{
				// Toggle the follow lock
				m_bPointerFollow = !m_bPointerFollow;
			}
			// Store the mouse's world position so it can be used later
			m_pointerPos = frame.mouseInput.worldPosition;
			return false;
		},
		true); // Pass true to force a callback every frame (unless overriden by a later registrant)

	m_hMainText = NewObject<GameObject>("MainText");
	auto pMainText = FindObject<GameObject>(m_hMainText);
	if (pMainText)
	{
		auto& prim0 = pMainText->GetPrim();
		prim0.Instantiate(Primitive::Type::Text);
		// Setup some text data
		TextData data("Press Tab to toggle pointer lock");
		data.oCharSize = 50;
		data.oFill = Colour(230, 125, 96); // {R, G, B, A} each E [0, 255]
		data.opFont = g_pResources->Find<Font>(m_hSerifFont);
		// Set the text
		prim0.SetText(data);
		// Let's move its position to +200 Y
		prim0.m_transform.SetPosition({0, 200});
	}

	m_hPointerCircle = NewObject<GameObject>("PointerCircle");
	auto pPointer = FindObject<GameObject>(m_hPointerCircle);
	if (pPointer)
	{
		ShapeData data;
		data.oSize = {10, 10};
		data.oFill = Colour(100, 100, 0);
		data.oOutline = Colour::Magenta;
		data.oBorder = 2;
		auto& pPointerPrim = pPointer->GetPrim();
		pPointerPrim.Instantiate(Primitive::Type::Circle)->SetShape(data);
	}
}

// We don't need (the compiler to store space for) dt right now, so comment it out to avoid compiler warnings
void Tutorial1::Tick(Time /*dt*/)
{
	if (m_bPointerFollow)
	{
		// Since this is called every frame, we just set the primitive's world position
		// to the mouse's when this frame began Ticking
		auto pPointer = FindObject<GameObject>(m_hPointerCircle);
		if (pPointer)
		{
			pPointer->GetPrim().m_transform.SetPosition(m_pointerPos);
		}
	}
}

void Tutorial1::OnStopping()
{
	// Reset the handles, just in case
	m_hMainText = m_hPointerCircle = INVALID_HANDLE;

	// Note m_bPointerFollow is not reset here (or in OnStarting()),
	// so it will retain its previous state if this world is reloaded

	// No need to reset m_hSerifFont: it won't be destroyed (it is not owned by this World, but by Resources)
}
} // namespace ME
