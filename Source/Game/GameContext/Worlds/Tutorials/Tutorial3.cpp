#include "Engine/GameServices.h"
#include "Tutorial3.h"
#include "../../Objects/Tutorials/Bubble.h"

namespace ME
{
void Tutorial3::OnStarting()
{
	m_hMainText = NewObject<GameObject>("MainText");
	auto pMainText = FindObject<GameObject>(m_hMainText);
	if (pMainText)
	{
		pMainText->Instantiate(Primitive::Type::Text);
		pMainText->SetText("Press [Tab] to spawn new bubbles\n[D] to destroy all").m_transform.SetPosition({0, 200});
	}

	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			if (frame.IsReleased(KeyCode::Space))
			{
				g_pContext->LoadWorld("Temp");
			}
			// Go back to Tutorial0 on Escape
			else if (frame.IsReleased(KeyCode::Escape))
			{
				g_pContext->LoadWorld("Tutorial2");
			}
			else if (frame.IsReleased(KeyCode::Tab))
			{
				std::string name = "Bubble_";
				name += std::to_string(m_bubbles.size());
				auto handle = NewObject<Bubble>(std::move(name));
				if (handle != INVALID_HANDLE)
				{
					auto pBubble = FindObject<Bubble>(handle);
					if (pBubble)
					{
						// Set a random time to live
						s32 ttlSecs = Maths::Random::Range(MIN_TTL_SECS, MAX_TTL_SECS);
						pBubble->m_ttl = Time::Seconds(static_cast<f32>(ttlSecs));
						// Scale the size and speed proportional to its ttl
						Fixed nSize = Fixed(ttlSecs) / Fixed(MAX_TTL_SECS);
						pBubble->m_diameter += (nSize * 20);
						pBubble->m_ySpeed -= (nSize * Fixed::OneHalf);
						// Set a random position in the world
						Fixed nX = Maths::Random::Range(-Fixed::One, Fixed::One);
						Fixed nY = -Fixed(0.8f);
						Vector2 worldPos = g_pGFX->WorldProjection({nX, nY});
						pBubble->m_transform.SetPosition(worldPos);
					}
					m_bubbles.push_back(handle);
				}
			}
			else if (frame.IsReleased(KeyCode::D))
			{
				DestroyAll(m_bubbles);
				m_bubbles.clear();
			}
			return false;
		},
		true);
}

void Tutorial3::Tick(Time dt)
{
	// Remove stale bubbles; use the type that the container holds for the template, and pass
	// the container and a predicate for each element in the container, that when returns true,
	// will remove the element from it.
	Core::RemoveIf<HObj>(m_bubbles, [this](HObj hBubble) { return FindObject<Bubble>(hBubble) == nullptr; });
	for (auto hBubble : m_bubbles)
	{
		if (auto pBubble = FindObject<Bubble>(hBubble))
		{
			// Fade the colour a bit
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
