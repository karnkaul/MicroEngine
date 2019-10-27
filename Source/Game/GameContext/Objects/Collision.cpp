#include "Engine/GameServices.h"
#include "Engine/Physics/Collider.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Rendering/Primitive.h"
#include "Engine/Rendering/Renderer.h"
#include "Collision.h"

namespace ME
{
Collision::Collision(GameObject& owner) : m_pOwner(&owner)
{
	m_signature = Maths::Random::Range(1, 10000);
}

Collision::~Collision()
{
	for (auto& data : m_colliders)
	{
#if defined(DEBUGGING)
		if (data.hPrim != INVALID_HANDLE)
		{
			g_pRenderer->Destroy(data.hPrim);
			data.hPrim = INVALID_HANDLE;
		}
#endif
		data.pCollider->m_bDestroyed = true;
	}
}

void Collision::Update(Vector2 position)
{
	for (auto& data : m_colliders)
	{
		data.pCollider->m_position = position + data.offset;
#if defined(DEBUGGING)
		if (auto pPrim = g_pRenderer->Find(data.hPrim))
		{
			pPrim->m_position = position + data.offset;
			pPrim->m_bEnabled = Collider::s_bShowDebugShape;
		}
#endif
	}
}

void Collision::SetEnabled(bool bEnabled)
{
	for (auto& data : m_colliders)
	{
		data.pCollider->m_bEnabled = bEnabled;
#if defined(DEBUGGING)
		if (auto pPrim = g_pRenderer->Find(data.hPrim))
		{
			pPrim->m_bEnabled = bEnabled && Collider::s_bShowDebugShape;
		}
#endif
	}
}

Collision::OnHit::Token Collision::AddCircle(OnHit::Callback onHit, Fixed diameter, Vector2 offset)
{
	CircleCollider* pCollider = g_pPhysics->CreateCircleCollider(std::string(m_pOwner->Name()));
	pCollider->SetCircle(diameter);
	SetupCollider(*pCollider);
#if defined(DEBUGGING)
	auto hCircle = g_pRenderer->New();
	if (auto pCircle = g_pRenderer->Find(hCircle))
	{
		pCircle->Instantiate(Primitive::Type::Circle);
		ShapeData data;
		data.oSize = {diameter * Fixed::OneHalf, diameter * Fixed::OneHalf};
		data.oBorder = Collider::s_debugShapeWidth;
		data.oOutline = Colour::Green;
		data.oFill = Colour::Transparent;
		pCircle->SetShape(data)->m_layer = 500;
		pCircle->m_bEnabled = Collider::s_bShowDebugShape;
	}
	Unit data{offset, pCollider, hCircle};
	m_colliders.emplace_back(std::move(data));
#else
	Unit data{offset, pCollider};
	m_colliders.emplace_back(std::move(data));
#endif
	return onHit ? m_onHit.Register(onHit) : Token();
}

Collision::OnHit::Token Collision::AddAABB(OnHit::Callback onHit, const AABBData& aabbData, Vector2 offset)
{
	AABBCollider* pCollider = g_pPhysics->CreateAABBCollider(std::string(m_pOwner->Name()));
	pCollider->SetAABB(aabbData);
	SetupCollider(*pCollider);
#if defined(DEBUGGING)
	auto hRect = g_pRenderer->New();
	if (auto pRect = g_pRenderer->Find(hRect))
	{
		pRect->Instantiate(Primitive::Type::Rectangle);
		ShapeData data;
		data.oSize = Fixed::Two * aabbData.upperBound;
		data.oBorder = Collider::s_debugShapeWidth;
		data.oOutline = Colour::Green;
		data.oFill = Colour::Transparent;
		pRect->SetShape(data)->m_layer = 500;
		pRect->m_bEnabled = Collider::s_bShowDebugShape;
	}
	Unit data{offset, pCollider, hRect};
	m_colliders.emplace_back(std::move(data));
#else
	Unit data{offset, pCollider};
	m_colliders.emplace_back(std::move(data));
#endif
	return onHit ? m_onHit.Register(onHit) : Token();
}

void Collision::RemoveAll()
{
	for (auto pCollider : m_colliders)
	{
		pCollider.pCollider->m_bDestroyed = true;
	}
	m_colliders.clear();
}

s32 Collision::Signature() const
{
	return m_signature;
}

void Collision::SetupCollider(Collider& collider)
{
	collider.m_ignoreSig = m_signature;
	collider.m_name += "_";
	collider.m_name += std::to_string(m_colliders.size());
	collider.m_instigator = m_pOwner->m_handle;
	collider.m_position = m_pOwner->m_transform.WorldPosition();
	collider.m_callback = [this](const Collider& lhs, const Collider& rhs) {
		auto pInstigator = GameWorld::Active().FindObject<GameObject>(rhs.m_instigator);
		m_onHit({m_pOwner, pInstigator, &lhs, &rhs});
	};
}
} // namespace ME
