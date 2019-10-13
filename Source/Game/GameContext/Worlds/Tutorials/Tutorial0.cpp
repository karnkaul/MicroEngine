#include "Tutorial0.h"
#include "Engine/GameServices.h" // Included to get global pointers to GameServices
#include "GameLoop/GameLoop.h"	 // To Stop the app

namespace ME
{
void Tutorial0::OnStarting()
{
	// Create a new GameObject (name is required, but can be duplicate), and store its handle
	m_hMainText = NewObject<GameObject>("MainText");
	// Obtain a pointer to the actual GameObject;
	auto pHelloText = FindObject<GameObject>(m_hMainText);
	// We ideally never want to store pointers, because this object may get destroyed by some other means
	// and then the pointer will dangle. That's one reason we use handles.
	if (pHelloText)
	{
		// Instantiate a text object: a Primitive can construct different kinds of "sub-primitives"
		pHelloText->Instantiate(Primitive::Type::Text);
		// Set some text
		pHelloText->SetText("... Press Space to Continue ...");
	}

	// This is how we register callbacks for user input: via a function
	// that takes in this parameter and returns a bool indicating whether it
	// should block this input pass (previous registrants will not receive a callback).
	RegisterInput([this](const Input::Frame& frame) -> bool {
		// Check if Space was released this frame (pressed => held => released)
		if (frame.IsReleased(KeyCode::Space))
		{
			// GameContext service owns all worlds and exposes an API to load a different one
			if (!g_pContext->LoadWorld("Tutorial1"))
			{
				// Log a warning to the console about something unexpected (not an error because it doesn't break anything)
				LOG_W("[%s] %s Tutorial1 GameWorld does not exist!", m_name.data(), m_type.data());
			}
		}
		// Let's quit the app if Escape is released!
		else if (frame.IsReleased(KeyCode::Escape))
		{
			LOG_I("[%s] %s Quitting instantly", m_name.data(), m_type.data());
			// Simple.
			GameLoop::Stop();
		}
		// We don't want to block any input, let lower callbacks get their turn
		return false;
	});
}

void Tutorial0::OnStopping()
{
	// Reset the handle to invalid (and all other state of this world),
	// effectively preparing it to be restarted again.
	m_hMainText = INVALID_HANDLE;
}
} // namespace ME
