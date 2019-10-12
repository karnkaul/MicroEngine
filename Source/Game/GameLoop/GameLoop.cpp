#include <bitset>
#include <thread>
#include "SFML/Graphics.hpp"
#include "Engine/GameServices.h"
#include "Engine/Viewport/Viewport.h"
#include "GameLoop.h"

namespace ME
{
namespace
{
constexpr u8 FPS_TARGET = 60;

struct State
{
	enum Flag
	{
		INIT = 0,
		CLOSED,
		QUITTING
	};
	static const Time tickRate;
	std::bitset<3> flags;

	bool Expired() const
	{
		return flags[CLOSED] || flags[QUITTING];
	}
};

const Time State::tickRate = Time::Seconds(1.0f / FPS_TARGET);
std::unique_ptr<GameServices> uGS;
State state;
std::string workingDir;

HPrim hprim0 = INVALID_HANDLE;
HPrim hprim1 = INVALID_HANDLE;

void Init(s32 argc, char** argv)
{
#if ENABLED(DEBUG_LOGGING)
	LE::g_MinLogSeverity = LE::LogSeverity::Debug;
#endif
#if ENABLED(ASSERTS)
	std::string msg;
	msg.reserve(16);
	msg += "Invalid argc! ";
	msg += std::to_string(argc);
	AssertVar(argc > 0, msg.c_str());
#endif
	if (argc > 0)
	{
		std::string_view exePath = argv[0];
#ifdef TARGET_WIN64
		const char delim = '\\';
#else
		const char delim = '/';
#endif
		workingDir = exePath.substr(0, exePath.find_last_of(delim));
		LOG_I("Working dir: %s", workingDir.data());
	}
	uGS = std::make_unique<GameServices>();
	g_pResources->s_resourcesPath = workingDir;
	g_pResources->s_resourcesPath += "/";
	g_pResources->s_resourcesPath += "Resources";
	if (g_pResources->Init({"Default-Mono.ttf", "Default-Serif.ttf"}))
	{
		state.flags[State::INIT] = true;
	}
}

void Create(Viewport& outVP, u32 width, u32 height, const std::string& title)
{
	ViewportData data;
	data.viewportSize.width = width;
	data.viewportSize.height = height;
	data.title = title;
	outVP.SetData(std::move(data));
	outVP.Create();
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(static_cast<f32>(width), static_cast<f32>(height)));
	outVP.setView(view);
}

void PollEvents(Viewport& vp)
{
	auto vpEvent = g_pInput->PollEvents(vp);
	switch (vpEvent)
	{
	default:
		break;

	case ViewportEventType::Closed:
		state.flags[State::CLOSED] = true;
		break;
	}
}

void Tick(Time dt)
{
	g_pInput->TakeSnapshot();
	g_pInput->FireCallbacks();

	// TODO @karnkaul: Update game state

	static bool bPrim0Created = false;
	if (hprim0 == INVALID_HANDLE && !bPrim0Created)
	{
		bPrim0Created = true;
		// Get a handle to a new Primitive and set it to hprim0
		hprim0 = g_pRenderer->New();
		// Get a pointer to the primitive handled by hprim0
		auto pPrim = g_pRenderer->Find(hprim0);
		if (pPrim)
		{
			// Instantiate a text object
			pPrim->Instantiate(Primitive::Type::Text);
			// Get a handle to the Default-Serif.ttf font
			auto hfont = g_pResources->Load<Font>("Default-Serif.ttf");
			// Setup some text data
			TextData data("Hello!");
			data.oCharSize = 100;
			data.opFont = g_pResources->Find<Font>(hfont);
			// Set the text
			pPrim->SetText(data);
			// Set the position to +200 in the y direction
			pPrim->m_transform.SetPosition({0, 200});
		}
	}

	static bool bPrim1Created = false;
	if (hprim1 == INVALID_HANDLE && !bPrim1Created)
	{
		bPrim1Created = true;
		// One liner
		auto pPrim = g_pRenderer->Find(hprim1 = g_pRenderer->New());
		// Skip cache-killing check (should always be true; else
		// your code is broken anyway). Assert instead
		Assert(pPrim, "Null pointer");
		ShapeData data;
		data.oSize = {500, 200};
		data.oFill = Colour(100, 100, 0);
		data.oOutline = Colour::Magenta;
		// One-liner
		pPrim->Instantiate(Primitive::Type::Rectangle)->SetShape(data);
		auto pPrim0 = g_pRenderer->Find(hprim0);
		if (pPrim0)
		{
			// Exploit matrix transformation to "lock" it to prim0
			pPrim->m_transform.SetParent(pPrim0->m_transform);
		}
	}

	// Only want these initialised once, hence "function static"
	static bool bLayerChanged = false;
	// Try changing this
	static Time layerRemain = Time::Seconds(1.5f);
	// Subtract elapsed time
	layerRemain -= dt;
	if (layerRemain <= Time::Zero && !bLayerChanged)
	{
		// Push the rectangle below the text after 1.5 seconds
		auto pPrim = g_pRenderer->Find(hprim1);
		if (pPrim)
		{
			--pPrim->m_layer;
			auto pPrim0 = g_pRenderer->Find(hprim0);
			if (pPrim0)
			{
				// Find a point halfway between centre and left edge
				Vector2 world = g_pGFX->WorldProjection({Fixed(-0.5f), 0});
				// Magic! Both move (because of parenting)
				pPrim0->m_transform.SetPosition(world);
				// Rotate only the child to point equal parts +x and +y
				// (all models start facing right (1, 0))
				pPrim->m_transform.SetOrientation(Vector2::One);
			}
		}
		// Stop decrementing layer! (Don't care about `remain` any more, that can
		// underflow for days / months / years / ...; won't affect any code)
		bLayerChanged = true;
	}

	static Time destroyRemain = Time::Seconds(3.5f);
	static bool bDestroyed = false;
	destroyRemain -= dt;
	if (destroyRemain <= Time::Zero && !bDestroyed)
	{
		// Destroy this one after 3.5 seconds
		g_pRenderer->Destroy(hprim1);
		hprim1 = INVALID_HANDLE;
	}
}

void Sleep(Time frameTime)
{
	Time residue = state.tickRate - frameTime;
	if (residue > Time::Zero)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(residue.AsMilliseconds()));
	}
}

void Cleanup()
{
	hprim0 = INVALID_HANDLE;
	hprim1 = INVALID_HANDLE;
	g_pRenderer->Clear();
	g_pResources->Clear();
	uGS = nullptr;
}
} // namespace

s32 GameLoop::Run(s32 argc, char** argv)
{
	if (!state.flags[State::INIT])
	{
		Init(argc, argv);
	}
	if (!state.flags[State::INIT])
	{
		LOG_E("[GameLoop] Fatal error initialising engine...");
		return 1;
	}
	state = State();
	Token t = g_pInput->Register([](const Input::Frame& frame) -> bool {
		LOGIF_I(frame.IsPressed(KeyCode::A), "A pressed!");
		return false;
	});
	Viewport viewport;
	ViewportSize size = g_pGFX->GetViewportSize();
	Create(viewport, size.width, size.height, "Untitled Game");
	Time frameStart = Time::Now();
	Time frameTime;
	while (viewport.isOpen() && !state.Expired())
	{
		Time dt = Time::Now() - frameStart;
		frameStart = Time::Now();
		PollEvents(viewport);
		Tick(dt);
		g_pRenderer->Render(viewport);
		frameTime = Time::Now() - frameStart;
		Sleep(frameTime);
	}
	Cleanup();
	return 0;
}

void GameLoop::Stop()
{
	state.flags[State::QUITTING] = true;
}

const std::string_view GameLoop::PWD()
{
	return workingDir;
}
} // namespace ME
