#include "GameServices.h"

namespace ME
{
Resources* g_pResources = nullptr;
Input* g_pInput = nullptr;
Renderer* g_pRenderer = nullptr;

GameServices::GameServices()
{
	Assert(g_pResources == nullptr, "Global pointer already registered!");
	g_pResources = &m_resources;
	Assert(g_pInput == nullptr, "Global pointer already registered!");
	g_pInput = &m_input;
	Assert(g_pRenderer == nullptr, "Global pointer already registered!");
	g_pRenderer = &m_renderer;
}

GameServices::~GameServices() 
{
	g_pResources = nullptr;
	g_pInput = nullptr;
	g_pRenderer = nullptr;
}
}
