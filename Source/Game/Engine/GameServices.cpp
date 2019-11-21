#include "GameServices.h"
#include "GameContext/GameInit.h"

namespace ME
{
GameContext* g_pContext = nullptr;
GFX* g_pGFX = nullptr;
Input* g_pInput = nullptr;
Physics* g_pPhysics = nullptr;
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
	Assert(g_pPhysics == nullptr, "Global pointer already registered!");
	g_pPhysics = &m_physics;
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
	g_pPhysics = nullptr;
	g_pResources = nullptr;
	g_pRenderer = nullptr;
}

void GameServices::StartGame()
{

	GameInit::CreateWorlds();
	m_context.StartWorld("Tutorial6");
}
} // namespace ME
