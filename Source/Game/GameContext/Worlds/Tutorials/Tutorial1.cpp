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
				g_pContext->LoadWorld("Tutorial2");
			}
			// Go back to Tutorial0 on Escape
			else if (frame.IsReleased(KeyCode::Escape))
			{
				g_pContext->LoadWorld("Tutorial0");
			}
			else if (frame.IsReleased(KeyCode::Tab))
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
		pMainText->Instantiate(Primitive::Type::Text);
		// Setup some custom text data
		TextData data("[Tab] : Toggle Pointer Lock\n[Space] : Load Next Tutorial\n[Escape] : Go Back");
		data.oCharSize = 50;
		data.oFill = Colour(230, 120, 90); // {R, G, B, A} each E [0, 255]
		data.opFont = g_pResources->Find<Font>(m_hSerifFont);
		// Set the text
		pMainText->SetText(data);
		// Let's move its position to +200 Y
		pMainText->m_transform.SetPosition(Vector2(0, 200));
	}

	m_hPointerCircle = NewObject<GameObject>("PointerCircle");
	auto pPointer = FindObject<GameObject>(m_hPointerCircle);
	if (pPointer)
	{
		ShapeData data;
		data.oSize = Vector2(30, 30);
		data.oFill = Colour(240, 220, 20);
		data.oOutline = Colour::Magenta;
		data.oBorder = 2;
		pPointer->Instantiate(Primitive::Type::Circle).SetShape(data);
		if (pMainText)
		{
			// Move the pointer a layer above the text's, so it's drawn on top of it
			pPointer->m_layer = pMainText->m_layer + 1;
		}
	}
}

// We don't need (the compiler to store space for) dt right now, so comment it out to avoid compiler warnings
void Tutorial1::Tick(Time dt)
{
	if (m_bPointerFollow)
	{
		// Since this is called every frame, we just set the primitive's world position
		// to the mouse's when this frame began Ticking
		auto pPointer = FindObject<GameObject>(m_hPointerCircle);
		if (pPointer)
		{
			pPointer->m_transform.SetPosition(m_pointerPos);
		}
	}
	// Remember to call the base class implementation (if it is not trivial)
	GameWorld::Tick(dt);
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
