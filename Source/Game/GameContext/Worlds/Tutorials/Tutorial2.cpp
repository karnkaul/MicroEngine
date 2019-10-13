#include "Engine/GameServices.h"
#include "Tutorial2.h"

namespace ME
{
void Tutorial2::OnCreated()
{
	Assert(g_pResources, "Resources is null!");
	// Get a handle to the Default-Serif.ttf font; load heavy stuff when created
	m_hSerifFont = g_pResources->Load<Font>("Default-Serif.ttf");
}

void Tutorial2::OnStarting()
{
	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			if (frame.IsReleased(KeyCode::Space))
			{
				const std::string nextWorldID = "Tutorial3";
				if (!g_pContext->LoadWorld(nextWorldID))
				{
					LOG_W("[%s] %s GameWorld does not exist!", m_name.data(), nextWorldID.data());
				}
			}
			// Go back to Tutorial0 on Escape
			if (frame.IsReleased(KeyCode::Escape))
			{
				g_pContext->LoadWorld("Tutorial1");
			}
			return false;
		},
		true);

	m_hObj0 = NewObject<GameObject>("Hello-Text");
	auto pObj0 = FindObject<GameObject>(m_hObj0);
	if (pObj0)
	{
		pObj0->Instantiate(Primitive::Type::Text);
		// Setup some text data
		TextData data("Hello!");
		data.oCharSize = 100;
		data.opFont = g_pResources->Find<Font>(m_hSerifFont);
		// Set the text
		pObj0->SetText(data);
		// Set the position to +200 in the y direction
		pObj0->m_transform.SetPosition({0, 200});
	}

	m_hObj1 = NewObject<GameObject>("Yellow-Rect");
	auto pObj1 = FindObject<GameObject>(m_hObj1);
	if (pObj1)
	{
		ShapeData data;
		data.oSize = {500, 200};
		data.oFill = Colour(100, 100, 0);
		data.oOutline = Colour::Magenta;
		pObj1->Instantiate(Primitive::Type::Rectangle).SetShape(data);
		if (pObj0)
		{
			// Exploit matrix transformation to "lock" it to prim0
			pObj1->m_transform.SetParent(pObj0->m_transform);
		}
	}

	// Reset all clocks etc
	static const Time LAYER_TTL = Time::Seconds(1.5f);
	static const Time PRIM_TTL = Time::Seconds(3.5f);
	m_obj1LayerTTL = LAYER_TTL;
	m_obj1TTL = PRIM_TTL;
	m_bObj1LayerChanged = m_bObj1Destroyed = false;
}

void Tutorial2::Tick(Time dt)
{
	m_obj1LayerTTL -= dt;
	if (m_obj1LayerTTL <= Time::Zero && !m_bObj1LayerChanged)
	{
		// Push the rectangle below the text after 1.5 seconds
		auto pObj1 = FindObject<GameObject>(m_hObj1);
		if (pObj1)
		{
			--pObj1->m_layer;
			auto pObj0 = FindObject<GameObject>(m_hObj0);
			if (pObj0)
			{
				// Find a point halfway between centre and left edge
				Vector2 world = g_pGFX->WorldProjection({Fixed(-0.5f), 0});
				// Magic! Both move (because of parenting)
				pObj0->m_transform.SetPosition(world);
				// Rotate only the child to point equal parts +x and +y
				// (all models start facing right (1, 0))
				pObj1->m_transform.SetOrientation(Vector2::One);
			}
		}
		// Stop decrementing layer! (Don't care about `remain` any more, that can
		// underflow for days / months / years / ...; won't affect any code)
		m_bObj1LayerChanged = true;
	}

	m_obj1TTL -= dt;
	if (m_obj1TTL <= Time::Zero && !m_bObj1Destroyed)
	{
		// Destroy this one after 3.5 seconds
		DestroyObject(m_hObj1);
		m_hObj1 = INVALID_HANDLE;
		m_bObj1Destroyed = true;
	}
	// Remember to call the base class implementation (if it is not trivial)
	GameWorld::Tick(dt);
}

void Tutorial2::OnStopping()
{
	// Reset the handles, just in case
	m_hObj0 = INVALID_HANDLE;
	m_hObj1 = INVALID_HANDLE;
}
} // namespace ME
