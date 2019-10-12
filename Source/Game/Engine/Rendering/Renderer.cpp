#include "Renderer.h"
#include "Engine/Viewport/Viewport.h"

namespace ME
{
Renderer::Renderer() = default;
Renderer::~Renderer()
{
	Clear();
}

void Renderer::Clear()
{
	m_factory.Clear();
}

HPrim Renderer::New()
{
	return m_factory.New<Primitive>();
}

Primitive* Renderer::Find(HPrim handle)
{
	if (auto pPrim = m_factory.Find<Primitive>(handle))
	{
		pPrim->m_handle = handle;
		return pPrim;
	}
	return nullptr;
}

bool Renderer::Destroy(HPrim handle)
{
	return m_factory.Destroy(handle);
}

void Renderer::Render(Viewport& viewport)
{
	std::vector<Primitive*> prims;
	prims.reserve(m_factory.m_instanced.size());
	for (auto& kvp : m_factory.m_instanced)
	{
		prims.push_back(&kvp.second);
	}
	std::sort(prims.begin(), prims.end(), [](Primitive* pLHS, Primitive* pRHS) { return pLHS->m_layer < pRHS->m_layer; });
	viewport.clear();
	for (auto pPrim : prims)
	{
		pPrim->Update();
		pPrim->Draw(viewport);
	}
	viewport.display();
}
} // namespace ME
