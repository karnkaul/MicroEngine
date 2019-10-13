#include "Engine/GameServices.h"
#include "GameObject.h"

namespace ME
{
GameObject::GameObject() = default;
GameObject::GameObject(GameObject&&) = default;
GameObject::~GameObject()
{
	g_pRenderer->Destroy(m_hPrim);
}

Primitive& GameObject::GetPrim()
{
	Assert(m_pPrim, "Primitive is null!");
	return *m_pPrim;
}

void GameObject::Destroy()
{
	m_bDestroyed = true;
}

void GameObject::RegisterInput(std::function<bool(const Input::Frame& frame)> callback)
{
	Assert(g_pInput, "Input is null!");
	m_inputTokens.push_back(g_pInput->Register(callback));
}

void GameObject::OnCreate() {}
void GameObject::Tick(Time /*dt*/) {}

void GameObject::Create(std::string id)
{
	m_name = std::move(id);
	m_pPrim = g_pRenderer->Find(m_hPrim = g_pRenderer->New());
	Assert(m_pPrim && m_hPrim != INVALID_HANDLE, "Could not allocate Primitive for GameObject!");
	OnCreate();
	LOG_D("[%s] %s created", m_name.data(), Type().data());
}
} // namespace ME
