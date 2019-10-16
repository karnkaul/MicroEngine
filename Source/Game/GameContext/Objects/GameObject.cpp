#include "Engine/GameServices.h"
#include "GameObject.h"

namespace ME
{
const Colour GameObject::DEFAULT_TEXT_COLOUR = Colour(255, 255, 255, 255);
const u32 GameObject::DEFAULT_TEXT_SIZE = 40;

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

GameObject& GameObject::Instantiate(Primitive::Type type)
{
	if (m_pPrim)
	{
		m_pPrim->Instantiate(type);
	}
	m_textData.oFill = DEFAULT_TEXT_COLOUR;
	m_textData.oCharSize = DEFAULT_TEXT_SIZE;
	Assert(!g_defaultFonts.empty(), "No default fonts!");
	m_textData.opFont = g_defaultFonts.front();
	return *this;
}

GameObject& GameObject::SetText(const TextData& data)
{
	Assert(m_pPrim, "Primitive is null!");
	if (data.oCharSize)
	{
		m_textData.oCharSize = data.oCharSize;
	}
	if (data.opFont && *data.opFont)
	{
		m_textData.opFont = data.opFont;
	}
	if (data.oText)
	{
		m_textData.oText = data.oText;
	}
	if (data.oFill)
	{
		m_textData.oFill = data.oFill;
	}
	if (data.oOutline)
	{
		m_textData.oOutline = data.oOutline;
	}
	if (data.oBorder)
	{
		m_textData.oBorder = data.oBorder;
	}
	m_pPrim->SetText(m_textData);
	return *this;
}

GameObject& GameObject::SetShape(const ShapeData& data)
{
	Assert(m_pPrim, "Primitive is null!");
	m_pPrim->SetShape(data);
	return *this;
}

void GameObject::SetEnabled(bool bEnabled)
{
	if (m_pPrim)
	{
		m_pPrim->m_bEnabled = bEnabled;
	}
	m_flags[ToIdx(Flags::Enabled)] = bEnabled;
}

void GameObject::Destroy()
{
	m_flags[ToIdx(Flags::Destroyed)] = true;
}

bool GameObject::IsEnabled() const
{
	return m_flags[ToIdx(Flags::Enabled)];
}

bool GameObject::IsDestroyed() const
{
	return m_flags[ToIdx(Flags::Destroyed)];
}

void GameObject::RegisterInput(std::function<bool(const Input::Frame& frame)> callback)
{
	Assert(g_pInput, "Input is null!");
	m_inputTokens.push_back(g_pInput->Register(callback));
}

void GameObject::OnCreate() {}
void GameObject::Tick(Time /*dt*/)
{
	if (m_pPrim)
	{
		m_pPrim->m_layer = m_layer;
		m_pPrim->m_position = m_transform.WorldPosition();
		m_pPrim->m_orientation = m_transform.WorldOrientation();
	}
}

void GameObject::Create(std::string id)
{
	m_name = std::move(id);
	m_pPrim = g_pRenderer->Find(m_hPrim = g_pRenderer->New());
	Assert(m_pPrim && m_hPrim != INVALID_HANDLE, "Could not allocate Primitive for GameObject!");
	SetEnabled(true);
	OnCreate();
	LOG_D("[%s] %s created", m_name.data(), Type().data());
}
} // namespace ME
