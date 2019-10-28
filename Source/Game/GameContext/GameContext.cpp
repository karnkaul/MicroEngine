#include "BuildVersion.h"
#include "GameContext.h"
#include "Engine/GameServices.h"

namespace ME
{
GameContext::GameContext() = default;
GameContext::GameContext(GameContext&&) = default;
GameContext::~GameContext() = default;

bool GameContext::LoadWorld(const std::string& id)
{
	if (m_worlds.empty())
	{
		LOG_E("[GameContext] No Gameworlds constructed!");
		return false;
	}
	auto search = m_worlds.find(id);
	if (search != m_worlds.end())
	{
		m_nextWorldID = id;
		return true;
	}
	LOG_W("[GameContext] %s GameWorld does not exist", id.data());
	return false;
}

bool GameContext::LoadPreviousWorld()
{
	if (!m_prevWorldID.empty())
	{
		m_nextWorldID = std::move(m_prevWorldID);
	}
	return false;
}

bool GameContext::StartWorld(const std::string& id)
{
#if defined(DEBUGGING)
	Assert(g_pRenderer, "Renderer is null!");
	TextData data;
	data.oFill = Colour::White;
	data.oCharSize = 14;
	Assert(!g_defaultFonts.empty(), "No default fonts!");
	data.opFont = g_defaultFonts.front();
	Vector2 pos = g_pGFX->WorldProjection({1, 1}) + Vector2(-80, -30);
	m_hFPS = g_pRenderer->New();
	if (auto pFPS = g_pRenderer->Find(m_hFPS))
	{
		pFPS->Instantiate(Primitive::Type::Text);
		data.oText = "0";
		pFPS->SetText(data)->m_position = pos;
		pFPS->m_layer = Layers::L3000_OVERLAY;
	}
	m_hFPSText = g_pRenderer->New();
	if (auto pText = g_pRenderer->Find(m_hFPSText))
	{
		pText->Instantiate(Primitive::Type::Text);
		data.oText = "FPS";
		pText->SetText(data)->m_position = pos + Vector2(25, 0);
		pText->m_layer = Layers::L3000_OVERLAY;
	}
	pos = g_pGFX->WorldProjection({-1, 1}) + Vector2(80, -30);
	m_hVersion = g_pRenderer->New();
	if (auto pVersion = g_pRenderer->Find(m_hVersion))
	{
		pVersion->Instantiate(Primitive::Type::Text);
		std::string versionText;
		versionText += BUILD_VERSION_STR;
		versionText += "\n";
		versionText += COMMIT_HASH_STR;
		data.oText = std::move(versionText);
		data.oFill = Colour(255, 200, 235);
		pVersion->SetText(data)->m_position = pos;
		pVersion->m_layer = Layers::L3000_OVERLAY;
	}
#endif
	if (m_worlds.empty())
	{
		LOG_E("[GameContext] No Gameworlds constructed!");
		return false;
	}
	auto search = m_worlds.find(id);
	if (search != m_worlds.end())
	{
		m_nextWorldID = id;
	}
	else
	{
		m_nextWorldID = m_worlds.begin()->second->m_name;
		LOG_W("[GameContext] [%s] Gameworld not found! Starting random GameWorld... [%s]", m_nextWorldID.data());
	}
	return true;
}

void GameContext::StartFrame()
{
	if (!m_nextWorldID.empty())
	{
		auto search = m_worlds.find(m_nextWorldID);
		Assert(search != m_worlds.end(), "Invalid WorldID!");
		if (m_pActive)
		{
			m_pActive->Stop();
			m_prevWorldID = m_pActive->m_name;
		}
		m_pActive = search->second.get();
		m_pActive->Start();
		m_nextWorldID.clear();
	}
}

void GameContext::Tick(Time dt)
{
	if (m_pActive)
	{
		m_pActive->Tick(dt);
	}
#if defined(DEBUGGING)
	static u8 frameCount = 0;
	static Time fpsElapsed;
	fpsElapsed += dt;
	++frameCount;
	if (fpsElapsed >= Time::Seconds(1.0f))
	{
		if (auto pFPS = g_pRenderer->Find(m_hFPS))
		{
			pFPS->SetText(std::to_string(frameCount));
		}
		frameCount = 0;
		fpsElapsed = Time::Zero;
	}
#endif
}

void GameContext::Stop()
{
	if (m_pActive)
	{
		m_pActive->Stop();
		m_pActive = nullptr;
	}
}

void GameContext::Clear()
{
	m_pActive = nullptr;
	m_worlds.clear();
	m_nextWorldID.clear();
	m_prevWorldID.clear();
}
} // namespace ME
