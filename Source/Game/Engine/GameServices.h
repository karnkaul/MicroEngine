#include "GameTypes.h"
#include "Engine/GFX.h"
#include "Engine/Input/Input.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Resources/Resources.h"

namespace ME
{
extern class GFX* g_pGFX;
extern class Input* g_pInput;
extern class Resources* g_pResources;
extern class Renderer* g_pRenderer;

class GameServices final
{
private:
	GFX m_gfx;
	Input m_input;
	Renderer m_renderer;
	Resources m_resources;

public:
	GameServices();
	~GameServices();
};
}