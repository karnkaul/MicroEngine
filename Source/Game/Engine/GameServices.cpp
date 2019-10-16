#include "GameServices.h"
#include "GameContext/Worlds/Worlds.h"

namespace ME
{
GameContext* g_pContext = nullptr;
GFX* g_pGFX = nullptr;
Input* g_pInput = nullptr;
Resources* g_pResources = nullptr;
Renderer* g_pRenderer = nullptr;

GameServices::GameServices()
{
	Assert(g_pContext == nullptr, "Global pointer already registered!");
	g_pContext = &m_context;
	Assert(g_pGFX == nullptr, "Global pointer already registered!");
	g_pGFX = &m_gfx;
	Assert(g_pInput == nullptr, "Global pointer already registered!");
	g_pInput = &m_input;
	Assert(g_pResources == nullptr, "Global pointer already registered!");
	g_pResources = &m_resources;
	Assert(g_pRenderer == nullptr, "Global pointer already registered!");
	g_pRenderer = &m_renderer;

	constexpr f64 WIDTH = 1280.0;
	constexpr u32 HEIGHT = 720;
	m_gfx.m_overrideNativeAR = WIDTH / HEIGHT;
	m_gfx.m_viewportHeight = m_gfx.m_worldHeight = HEIGHT;
	m_gfx.Init();
}

GameServices::~GameServices()
{
	g_pContext = nullptr;
	g_pGFX = nullptr;
	g_pInput = nullptr;
	g_pResources = nullptr;
	g_pRenderer = nullptr;
}

void GameServices::StartGame()
{
	m_context.ConstructWorld<Tutorial0>("Tutorial0");
	m_context.ConstructWorld<Tutorial1>("Tutorial1");
	m_context.ConstructWorld<Tutorial2>("Tutorial2");
	m_context.ConstructWorld<Tutorial3>("Tutorial3");
	m_context.ConstructWorld<Temp>("Temp");
	m_context.StartWorld("Tutorial0");
}

void GameServices::StartFrame()
{
	m_context.StartFrame();
}

void GameServices::Tick(Time dt)
{
	m_context.Tick(dt);
}

void GameServices::Stop()
{
	m_context.Stop();
}
} // namespace ME
