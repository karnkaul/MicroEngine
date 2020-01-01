#include "Engine/GameServices.h"
#include "Rocket.h"
#include "Bubble.h"

namespace ME
{
void Rocket::OnCreate()
{
	// Base class is Chaser, not GameObject!
	Chaser::OnCreate();

	m_hShipTex = g_pResources->Load<Texture>("Textures/WhiteSpaceShip_128x128.png");
	if (auto pTexture = g_pResources->Find<Texture>(m_hShipTex))
	{
		// New Primtive type: Sprite - uses a texture to render itself
		Instantiate(Primitive::Type::Sprite);
		SpriteData data(*pTexture);
		// 128x128 is too large, reduce it to 64x64; try 128x128 to see what it's like
		data.oScale = {Fixed(0.75f), Fixed(0.75f)};
		SetSprite(data);
		// Check out this texture: you'll notice it's like a flip-book
		m_hExhaustTex = g_pResources->Load<Texture>("Textures/Exhaust_128x128_2x4.png");
		if (auto pExhaustTex = g_pResources->Find<Texture>(m_hExhaustTex))
		{
			// Spritesheet::Autobuild takes in a texture (to obtain its size), and the number of columns and rows
			m_exhaustFrames.Autobuild(*pExhaustTex, 2, 4);
			// Period is the TOTAL time for ALL frames to play;
			// basically the index will return to 0 in this much time.
			m_exhaustFrames.m_period = Time::Milliseconds(250);
			// Obtain a reference to the world (not const, because `NewObject<T>()` will modify it)
			auto& world = GameWorld::Active(); // This call will Assert if there is no active GameWorld
			m_hExhaust = world.NewObject<GameObject>("Exhaust");
			if (auto pExhaust = world.FindObject<GameObject>(m_hExhaust))
			{
				pExhaust->Instantiate(Primitive::Type::Sprite);
				SpriteData data(*pExhaustTex);
				// Set the texture coordinates to whatever the spritesheet's index points to
				data.oTexCoords = m_exhaustFrames.m_texCoords[m_exhaustFrames.m_index];
				pExhaust->SetSprite(data);
				// Move the exhaust a bit to the left of the ship (when both are facing Right)
				const Fixed x = -(pExhaust->Bounds().Size().x * Fixed::OneHalf + 20);
				pExhaust->m_transform.SetPosition({x, 0});
				// Set the exhaust as a "child" transform of the rocket;
				// this will automatically apply the parent's transformation to the child.
				// Ask for more details about this; it's a complicated topic.
				// Roughly speaking, `SetParent()` will make an object "stick" to another (in 2D space).
				pExhaust->m_transform.SetParent(m_transform);
			}
		}

		m_collisionToken = m_collision.AddAABB(nullptr, AABBData({Bounds().Size() * Fixed::OneHalf}));
	}
}

void Rocket::Tick(Time dt)
{
	if (auto pExhaust = GameWorld::Active().FindObject<GameObject>(m_hExhaust))
	{
		// Rocket is also a GameObject, so its layer might have been changed by
		// some owning World/Object after OnCreate / on some other Tick;
		// therefore update all sub-object layers every Tick here.
		pExhaust->m_layer = m_layer;
		// Spritesheet Tick moves its clock forward and returns true if its index has changed
		if (m_exhaustFrames.Tick(dt))
		{
			SpriteData data;
			// Update the texture coordinates
			data.oTexCoords = m_exhaustFrames.m_texCoords[m_exhaustFrames.m_index];
			pExhaust->SetSprite(data);
		}
	}

	// Base class is Chaser, not GameObject!
	Chaser::Tick(dt);
}

void Rocket::OnDestroy()
{
	if (auto pExhaust = GameWorld::Active().FindObject<GameObject>(m_hExhaust))
	{
		// Destroy exhaust too
		pExhaust->Destroy();
	}
}
} // namespace ME
