#include "Engine/GameServices.h"
#include "../../Objects/Tutorials/Rocket.h"
#include "Tutorial5.h"

namespace ME
{
void Tutorial5::OnCreated()
{
	// A Texture is an image that lives on the GPU memory; it is extremely
	// fast and performant when in use, but is *very* slow to create.
	// Check the texture out in Resources: you'll notice it faces right; this
	// is because by convention our "default" orientation in the world is `Vector2::Right` (1, 0).
	m_hShipTex = g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
}

void Tutorial5::OnStarting()
{
	RegisterInput([](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Temp");
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial4");
		}
		return false;
	});

	m_hMainText = NewObject<GameObject>("MainText");
	if (auto pText = FindObject<GameObject>(m_hMainText))
	{
		pText->Instantiate(Primitive::Type::Text);
		pText->SetText("Press [Tab] to toggle chasing");
	}

	if (auto pTexture = g_pResources->Find<Texture>(m_hShipTex))
	{
		m_hRocket = NewObject<Rocket>("Rocket");
		if (auto pRocket = FindObject<Rocket>(m_hRocket))
		{
			// New Primtive type: Sprite - uses a texture to render itself
			pRocket->Instantiate(Primitive::Type::Sprite);
			SpriteData data(*pTexture);
			// 128x128 is too large, reduce it to 64x64
			data.oScale = {Fixed::OneHalf, Fixed::OneHalf};
			pRocket->SetSprite(data);
			pRocket->m_layer = 10;
		}
	}
}

void Tutorial5::Tick(Time dt)
{
	if (auto pRocket = FindObject<Rocket>(m_hRocket))
	{
		SpriteData data;
		// Override only the colour, based on Rocket's state
		data.oFill = pRocket->m_state == Rocket::State::Chasing ? Colour::Red : Colour::White;
		pRocket->SetSprite(data);
	}
	GameWorld::Tick(dt);
}

void Tutorial5::OnStopping()
{
	m_hRocket = m_hMainText = INVALID_HANDLE;
}
}
