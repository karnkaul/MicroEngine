#pragma once
#include "Factory/Factory.h"
#include "GameTypes.h"
#include "Primitive.h"

namespace ME
{
using HPrim = s32;

class Renderer final : NoCopy
{
private:
	Factory<Primitive> m_factory;

public:
	Renderer();
	~Renderer();

public:
	void Clear();

	HPrim New();
	Primitive* Find(HPrim handle);
	bool Destroy(HPrim handle);

	void Render(Viewport& viewport);
};
} // namespace ME
