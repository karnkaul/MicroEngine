#include <thread>
#include "App.h"
#include "OS.h"
#if defined(DEBUGGING)
#include "Engine/Physics/Collider.h"
#endif

namespace ME
{
App::App(u8 minFPS, u8 maxFPS) : m_TICK_RATE(Time::Seconds(1.0f / maxFPS)), m_MAX_DT(Time::Seconds(1.0f / minFPS))
{
	if (g_pResources->Init({"Default-Mono.ttf", "Default-Serif.ttf"}))
	{
		m_state.flags[State::INIT] = true;
	}

#if defined(DEBUGGING)
	m_debugTokens.push_back(g_pInput->RegisterSudo([](const Input::Frame& frame) {
		if (frame.IsHeld(KeyCode::LShift) && frame.IsReleased(KeyCode::C))
		{
			Collider::s_bShowDebugShape = !Collider::s_bShowDebugShape;
		}
		return false;
	}));
#endif
}

App::~App()
{
	// Stop any active Worlds
	g_pContext->Stop();
	// Clear all primitives
	g_pRenderer->Clear();
	// Clear all resources
	g_pResources->Clear();
	// Destroy all Worlds
	g_pContext->Clear();
	LOG_I("[App] terminated and destroyed");
}

void App::CreateViewport(const ViewportData& data)
{
	Assert(!m_viewport.isOpen(), "Viewport already open!");
	m_viewport.SetData(data);
	m_viewport.Create();
}

void App::StartGame()
{
	m_services.StartGame();
}

void App::StartFrame()
{
	g_pContext->StartFrame();
}

void App::PollEvents()
{
	auto vpEvent = g_pInput->PollEvents(m_viewport);
	switch (vpEvent)
	{
	default:
		break;

	case ViewportEventType::Closed:
		m_state.flags[State::CLOSED] = true;
		break;
	}
}

void App::Tick(Time dt)
{
	g_pInput->TakeSnapshot();
	g_pInput->FireCallbacks();
	g_pContext->Tick(dt);
}

void App::Render()
{
	g_pRenderer->Render(m_viewport);
}

void App::Sleep(Time frameTime)
{
	Time residue = m_TICK_RATE - frameTime;
	if (residue > Time::Zero)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(residue.AsMilliseconds()));
	}
}

void App::Quit()
{
	m_state.flags[State::Flag::CLOSED] = true;
}

bool App::IsInit() const
{
	return m_state.flags[State::INIT];
}

bool App::IsRunning() const
{
	return m_state.flags[State::INIT] && !m_state.flags[State::QUITTING] && !m_state.flags[State::CLOSED] && m_viewport.isOpen();
}

bool App::IsViewportOpen() const
{
	return m_viewport.isOpen();
}
} // namespace ME
