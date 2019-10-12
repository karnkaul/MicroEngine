#include "Engine/GameServices.h"
#include "DemoWorld.h"

namespace ME
{
void DemoWorld::OnCreated() 
{
	Assert(g_pResources, "Resources is null!");
	// Get a handle to the Default-Serif.ttf font; load heavy stuff when created
	m_hSerifFont = g_pResources->Load<Font>("Default-Serif.ttf");
}

void DemoWorld::Start() 
{
	m_hPrim0 = g_pRenderer->New();
	// Get a pointer to the primitive handled by m_hprim0
	auto pPrim = g_pRenderer->Find(m_hPrim0);
	if (pPrim)
	{
		// Instantiate a text object
		pPrim->Instantiate(Primitive::Type::Text);
		// Setup some text data
		TextData data("Hello!");
		data.oCharSize = 100;
		data.opFont = g_pResources->Find<Font>(m_hSerifFont);
		// Set the text
		pPrim->SetText(data);
		// Set the position to +200 in the y direction
		pPrim->m_transform.SetPosition({0, 200});
	}
	
	// One liner
	pPrim = g_pRenderer->Find(m_hPrim1 = g_pRenderer->New());
	// Skip cache-killing check (should always be true; else
	// your code is broken anyway). Assert instead
	Assert(pPrim, "Null pointer");
	ShapeData data;
	data.oSize = {500, 200};
	data.oFill = Colour(100, 100, 0);
	data.oOutline = Colour::Magenta;
	// One-liner
	pPrim->Instantiate(Primitive::Type::Rectangle)->SetShape(data);
	// Find previous primitive
	auto pPrim0 = g_pRenderer->Find(m_hPrim0);
	if (pPrim0)
	{
		// Exploit matrix transformation to "lock" it to prim0
		pPrim->m_transform.SetParent(pPrim0->m_transform);
	}

	// Reset all clocks etc
	static const Time LAYER_TTL = Time::Seconds(1.5f);
	static const Time PRIM_TTL = Time::Seconds(3.5f);
	m_prim1LayerTTL = LAYER_TTL;
	m_prim1TTL = PRIM_TTL;
}

void DemoWorld::Tick(Time dt)
{
	m_prim1LayerTTL -= dt;
	if (m_prim1LayerTTL <= Time::Zero && !m_bPrim1LayerChanged)
	{
		// Push the rectangle below the text after 1.5 seconds
		auto pPrim1 = g_pRenderer->Find(m_hPrim1);
		if (pPrim1)
		{
			--pPrim1->m_layer;
			auto pPrim0 = g_pRenderer->Find(m_hPrim0);
			if (pPrim0)
			{
				// Find a point halfway between centre and left edge
				Vector2 world = g_pGFX->WorldProjection({Fixed(-0.5f), 0});
				// Magic! Both move (because of parenting)
				pPrim0->m_transform.SetPosition(world);
				// Rotate only the child to point equal parts +x and +y
				// (all models start facing right (1, 0))
				pPrim1->m_transform.SetOrientation(Vector2::One);
			}
		}
		// Stop decrementing layer! (Don't care about `remain` any more, that can
		// underflow for days / months / years / ...; won't affect any code)
		m_bPrim1LayerChanged = true;
	}

	m_prim1TTL -= dt;
	if (m_prim1TTL <= Time::Zero && !m_bPrim1Destroyed)
	{
		// Destroy this one after 3.5 seconds
		g_pRenderer->Destroy(m_hPrim1);
		m_hPrim1 = INVALID_HANDLE;
	}
}

void DemoWorld::Stop() 
{
	// Clear the screen of whatever this world instantiated
	g_pRenderer->Destroy(m_hPrim0);
	g_pRenderer->Destroy(m_hPrim1);
	// Reset the handles, just in case
	m_hPrim0 = m_hPrim1 = INVALID_HANDLE;
}
} // namespace ME
