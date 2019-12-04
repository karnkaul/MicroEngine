#pragma once
#include "Engine/Rendering/Spritesheet.h"
#include "Chaser.h" // Check out this class first

namespace ME
{
// Derived class that sets up all the visuals for the base Rocket
class Rocket : public Chaser
{
protected:
	// New class! A Sprite can be asked to render only a part of a texture;
	// the rectangle to render is specified via TexCoords (top, left, width, height).
	// Spritesheet contains multiple sequential TexCoords associated with one texture size,
	// and also supports automatically incrementing the current index based on
	// the configured "period" of all frames to animate. (Columns, then rows.)
	Spritesheet m_exhaustFrames;
	HRes m_hShipTex = INVALID_HANDLE;
	// A "sprite sheet" (the general term, and also the class) is still one texture (image)!
	HRes m_hExhaustTex = INVALID_HANDLE;
	// This rocket instance will ask its owning world to instantiate another GameObject, to
	// use for the exhaust.
	HObj m_hExhaust = INVALID_HANDLE;
	Token m_collisionToken;

protected:
	void OnCreate() override;
	void Tick(Time dt) override;
	void OnDestroy() override;
};
} // namespace ME
