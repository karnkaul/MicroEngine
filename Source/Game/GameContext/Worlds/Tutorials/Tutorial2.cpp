#include <array>
#include "Engine/GameServices.h"
#include "Tutorial2.h"

namespace ME
{
void Tutorial2::OnStarting()
{
	RegisterInput([this](const Input::Frame& frame) -> bool {
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
		if (frame.IsReleased(KeyCode::Tab) && m_hRectangle != INVALID_HANDLE)
		{
			// If we'd stored pRect, it would dangle after it got destroyed!
			auto pRect = FindObject<GameObject>(m_hRectangle);
			// This is a fresh pointer, it will reflect the current state of the object
			if (pRect)
			{
				pRect->SetEnabled(!pRect->IsEnabled());
			}
		}
		return false;
	});

	m_hMainText = NewObject<GameObject>("MainText");
	auto pMainText = FindObject<GameObject>(m_hMainText);
	if (pMainText)
	{
		pMainText->Instantiate(Primitive::Type::Text);
		TextData data("Press Tab to toggle the Rectangle");
		data.oCharSize = 50;
		data.oFill = Colour::Cyan;
		pMainText->SetText(data);
		// Set this text at halfway between the centre and the bottom.
		// We can get that point by using a space projection: takes in a normalised
		// value between  [-1, 1] (0 being in the centre) for each axis and returns
		// the scaled/projected Vector2 in that world space.
		auto pos = g_pGFX->WorldProjection(Vector2(0, -Fixed::OneHalf));
		pMainText->m_transform.SetPosition(pos);
	}

	CreateRect();

	// Set (or reset) the rectangle's time to live to the constant
	s32 randomMilliseconds = Maths::Random::Range(m_RECT_TTL_MIN, m_RECT_TTL_MAX_S);
	m_rectTTL = Time::Milliseconds(randomMilliseconds);
	m_rectElapsed = Time::Zero;
}

void Tutorial2::Tick(Time dt)
{
	// Decrement elapsed time (deltaTime) from rect's ttl
	m_rectTTL -= dt;
	// Add dt to how long the rect has been alive
	m_rectElapsed += dt;	
	if (m_rectTTL <= Time::Zero && m_hRectangle != INVALID_HANDLE)
	{
		// Time is up! Destroy the rect
		DestroyObject(m_hRectangle);
		m_hRectangle = INVALID_HANDLE;

		// Set the main text
		auto pMainText = FindObject<GameObject>(m_hMainText);
		if (pMainText)
		{
			// It's just easier to use sprintf than mangle about with std::fixed,
			// to fully control the output format
			std::array<char, 8> buf;
			SPRINTF(buf.data(), buf.size(), "%.1f", m_rectElapsed.AsSeconds());
			// Although this whole string could be constructed in one line,
			// appending disparate chunks like this is much more efficient
			std::string text = "Rect is ded... (in ";
			text += buf.data();
			text += " seconds)";
			pMainText->SetText(text);
		}
	}
	GameWorld::Tick(dt);
}

void Tutorial2::OnStopping()
{
	m_hMainText = m_hRectangle = INVALID_HANDLE;
}

void Tutorial2::CreateRect()
{
	m_hRectangle = NewObject<GameObject>("Rectangle");
	auto pRect = FindObject<GameObject>(m_hRectangle);
	if (pRect)
	{
		pRect->Instantiate(Primitive::Type::Rectangle);
		ShapeData data;
		data.oSize = Vector2(200, 100); // x = width, y = height or x + y = diameter
		data.oFill = Colour(210, 110, 250);
		pRect->SetShape(data);
	}
}
} // namespace ME
