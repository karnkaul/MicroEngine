#include "Engine/GameServices.h"
#include "Temp.h"
#include "../../Objects/UI/UIButton.h"

namespace ME
{
void Temp::OnCreated()
{
	Assert(g_pResources, "Resources is null!");
	// Get a handle to the Default-Serif.ttf font; load heavy stuff when created
	m_hSerifFont = g_pResources->Load<Font>("Default-Serif.ttf");
}

void Temp::OnStarting()
{
	RegisterInput(
		[](const Input::Frame& frame) -> bool {
			if (frame.IsReleased(KeyCode::Space))
			{
				LOG_D("Temp world not configured to load anything beyond it");
			}
			// Go back to Tutorial0 on Escape
			else if (frame.IsReleased(KeyCode::Escape))
			{
				g_pContext->LoadPreviousWorld();
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

	m_hObj1 = NewObject<GameObject>("Ship-Sprite");
	auto hSpaceShip = g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	if (auto pObj1 = FindObject<GameObject>(m_hObj1))
	{
		auto pTex = g_pResources->Find<Texture>(hSpaceShip);
		Assert(pTex, "Texture is null!");
		SpriteData data(*pTex);
		TexCoords coords(pTex->Size());
		coords.SetUV(0, 0, Fixed(0.5f), Fixed(0.5f));
		data.oTexCoords = coords;
		pObj1->Instantiate(Primitive::Type::Sprite).SetSprite(data);
		if (pObj0)
		{
			// Exploit matrix transformation to "lock" it to prim0
			pObj1->m_transform.SetParent(pObj0->m_transform);
			// Move this object on top (rendered last)
			++pObj1->m_layer;
		}
	}

	m_hObj2 = NewObject<UIButton>("Test-Button");
	if (auto pBtn = FindObject<UIButton>(m_hObj2))
	{
		pBtn->SetUIText("Click!").m_transform.SetPosition({100, 100});
		m_tokens.push_back(pBtn->Register([]() { LOG_I("Button pressed!"); }));
	}

	// Reset all clocks etc
	static const Time LAYER_TTL = Time::Seconds(1.5f);
	static const Time PRIM_TTL = Time::Seconds(3.5f);
	m_obj1LayerTTL = LAYER_TTL;
	m_obj1TTL = PRIM_TTL;
	m_bObj1LayerChanged = m_bObj1Destroyed = false;
}

void Temp::Tick(Time dt)
{
	m_obj1LayerTTL -= dt;
	if (m_obj1LayerTTL <= Time::Zero && !m_bObj1LayerChanged)
	{
		// Push the rectangle below the text after 1.5 seconds
		auto pObj1 = FindObject<GameObject>(m_hObj1);
		if (pObj1)
		{
			// Now move it to the bottom
			pObj1->m_layer -= 2;
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

void Temp::OnStopping()
{
	// Reset the handles, just in case
	m_hObj0 = INVALID_HANDLE;
	m_hObj1 = INVALID_HANDLE;
	m_hObj2 = INVALID_HANDLE;
}
} // namespace ME
