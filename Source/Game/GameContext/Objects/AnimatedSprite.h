#pragma once
#include "GameObject.h"
#include "Engine/Rendering/Spritesheet.h"

namespace ME
{
class AnimatedSprite : public GameObject
{
public:
	Spritesheet m_sheet;
	bool m_bAnimate = false;
	bool m_bSelfDestruct = true;

protected:
	void Tick(Time dt) override;

public:
	void ResetFrame();
};
} // namespace ME
