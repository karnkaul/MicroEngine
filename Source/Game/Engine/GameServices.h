#pragma once
#include "GameTypes.h"
#include "Engine/GFX.h"
#include "Engine/Input/Input.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Resources/Resources.h"
#include "GameContext/GameContext.h"

namespace ME
{
extern class GFX* g_pGFX;
extern class Input* g_pInput;
extern class Resources* g_pResources;
extern class Renderer* g_pRenderer;
extern class GameContext* g_pContext;

class GameServices final
{
private:
	GameContext m_context;
	GFX m_gfx;
	Input m_input;
	Renderer m_renderer;
	Resources m_resources;

public:
	GameServices();
	~GameServices();

public:
	void StartGame();
};
} // namespace ME
