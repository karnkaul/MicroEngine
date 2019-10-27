#include "Engine/GameServices.h"
#include "GameObject.h"

namespace ME
{
const Colour GameObject::DEFAULT_TEXT_COLOUR = Colour(255, 255, 255, 255);
const u32 GameObject::DEFAULT_TEXT_SIZE = 40;

GameObject::GameObject() : m_collision(*this) {}
GameObject::GameObject(GameObject&&) = default;
GameObject::~GameObject()
{
	g_pRenderer->Destroy(m_hPrim);
	LOG_D("[%s] %s (%d) destroyed", m_name.data(), m_type.data(), m_handle);
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

GameObject& GameObject::SetSprite(const SpriteData& data)
{
	Assert(m_pPrim, "Primitive is null!");
	m_pPrim->SetSprite(data);
	return *this;
}

Collision& GameObject::GetCollision()
{
	return m_collision;
}

const Collision& GameObject::GetCollision() const
{
	return m_collision;
}

void GameObject::SetEnabled(bool bEnabled)
{
	if (m_pPrim)
	{
		m_pPrim->m_bEnabled = bEnabled;
	}
	if (bEnabled)
	{
		m_flags[ToIdx(Flags::Despawned)] = false;
		this->GameObject::Tick(Time::Zero);
	}
	m_collision.SetEnabled(bEnabled);
	m_flags[ToIdx(Flags::Enabled)] = bEnabled;
}

void GameObject::Destroy()
{
	if (m_pPool)
	{
		SetEnabled(false);
		m_flags[ToIdx(Flags::Despawned)] = true;
	}
	else
	{
		m_flags[ToIdx(Flags::Destroyed)] = true;
	}
	OnDestroy();
}

bool GameObject::IsEnabled() const
{
	return m_flags[ToIdx(Flags::Enabled)];
}

bool GameObject::IsDestroyed() const
{
	return m_flags[ToIdx(Flags::Destroyed)];
}

Rect2 GameObject::Bounds() const
{
	return m_pPrim->Bounds();
}

void GameObject::OnCreate() {}
void GameObject::OnRespawn() {}

void GameObject::Tick(Time /*dt*/)
{
	if (m_pPrim)
	{
		m_pPrim->m_layer = m_layer;
		m_pPrim->m_position = m_transform.WorldPosition();
		m_pPrim->m_orientation = m_transform.WorldOrientation();
	}
	m_collision.Update(m_transform.WorldPosition());
}

void GameObject::OnDestroy() {}

void GameObject::Create(std::string name)
{
	m_name = std::move(name);
	m_pPrim = g_pRenderer->Find(m_hPrim = g_pRenderer->New());
	Assert(m_pPrim && m_hPrim != INVALID_HANDLE, "Could not allocate Primitive for GameObject!");
	OnCreate();
	SetEnabled(true);
	LOG_D("[%s] %s created", m_name.data(), Type().data());
}

void GameObject::Respawn(std::string name)
{
	m_name = std::move(name);
	OnRespawn();
	SetEnabled(true);
}
} // namespace ME
