#include "Engine/GameServices.h"
#include "Tutorial3.h"
#include "../../Objects/Tutorials/Bubble.h" // Check out this class first

namespace ME
{
/**
 * Note: the approach used here, of constructing and destroying new objects arbitrarily, is not
 * meant to be used in real game code - such small allocations fragment the memory over time
 * (especially on runtimes with simple allocators). There are many ways to solve this problem,
 * one of the easiest and most popular ones being that of an object pool: create the max number you
 * will need, enable/disable them as required.
 */
void Tutorial3::OnStarting()
{
	m_hMainText = NewObject<GameObject>("MainText");
	// variables can be initialised in if statements; their scope is limited to the if block
	if (auto pMainText = FindObject<GameObject>(m_hMainText))
	{
		pMainText->Instantiate(Primitive::Type::Text);
		pMainText->SetText("Press [Tab] to spawn new bubbles\n[D] to destroy all").m_transform.SetPosition({0, 200});
		// Ensure bubbles appear below text
		m_bubbleLayer = pMainText->m_layer - 1;
	}

	RegisterInput([this](const Input::Frame& frame) -> bool {
		if (frame.IsReleased(KeyCode::Space))
		{
			g_pContext->LoadWorld("Tutorial4");
		}
		else if (frame.IsReleased(KeyCode::Escape))
		{
			g_pContext->LoadWorld("Tutorial2");
		}
		else if (frame.IsReleased(KeyCode::Tab))
		{
			std::string name = "Bubble_";
			name += std::to_string(m_bubbles.size());
			auto handle = NewObject<Bubble>(std::move(name));
			if (auto pBubble = FindObject<Bubble>(handle))
			{
				// Set a random time to live
				const s32 ttlSecs = Maths::Random::Range(MIN_TTL_SECS, MAX_TTL_SECS);
				pBubble->m_ttl = Time::Seconds(static_cast<f32>(ttlSecs));
				// Scale the size and speed proportional to its ttl
				const Fixed nSize = Fixed(ttlSecs) / Fixed(MAX_TTL_SECS);
				pBubble->m_diameter += (nSize * 20);
				pBubble->m_ySpeed -= (nSize * Fixed::OneHalf);
				// Set a random x in the world
				const Fixed nX = Maths::Random::Range(-Fixed::One, Fixed::One);
				// Y near the bottom
				const Fixed nY = -Fixed(0.8f);
				pBubble->m_transform.SetPosition(g_pGFX->WorldProjection({nX, nY}));
				pBubble->m_layer = m_bubbleLayer;
			}
			m_bubbles.push_back(handle);
		}
		else if (frame.IsReleased(KeyCode::D))
		{
			// This function takes in a vector of handles, destroys all
			// `GameObject`s they point to and sets them to INVALID_HANDLE
			DestroyAll(m_bubbles);
			m_bubbles.clear();
		}
		return false;
	});
}

void Tutorial3::Tick(Time dt)
{
	// Remove stale bubbles; use the type that the container holds for the template, and pass
	// the container and a predicate for each element in the container, that when returns true,
	// will remove the element from it.
	// `FindObject<T>()` is a member function, so the lambda requires `this` to be captured
	Core::RemoveIf<HObj>(m_bubbles, [this](HObj hBubble) { return FindObject<Bubble>(hBubble) == nullptr; });
	for (auto hBubble : m_bubbles)
	{
		if (auto pBubble = FindObject<Bubble>(hBubble))
		{
			// Fade the colour's alpha a bit
			pBubble->m_colour.a -= 1;
		}
	}
	GameWorld::Tick(dt);
}

void Tutorial3::OnStopping()
{
	// Clear all handles
	m_bubbles.clear();
	m_hMainText = INVALID_HANDLE;
}
} // namespace ME
