#include "Collider.h"
#include "Physics.h"

namespace ME
{
namespace
{
bool IgnoreSignatures(s32 lhs, s32 rhs)
{
	return lhs != 0 && rhs != 0 && lhs == rhs;
}
} // namespace
Physics::Physics() = default;
Physics::~Physics()
{
	Clear();
}

void Physics::Clear()
{
	m_colliders.clear();
}

void Physics::Update()
{
	Scrub();

	for (size_t i = 0; i < m_colliders.size(); ++i)
	{
		auto& lhs = m_colliders[i];
		if (!lhs->m_bEnabled)
		{
			return;
		}
		for (size_t j = i + 1; j < m_colliders.size(); ++j)
		{
			auto& rhs = m_colliders[j];
			if (!rhs->m_bEnabled || IgnoreSignatures(lhs->m_ignoreSig, rhs->m_ignoreSig))
			{
				continue;
			}
			if (lhs->IsIntersecting(*rhs))
			{
				lhs->OnHit(*rhs);
				rhs->OnHit(*lhs);
			}
		}
	}
}

CircleCollider* Physics::CreateCircleCollider(std::string ownerName)
{
	auto uCollider = std::make_unique<CircleCollider>(std::move(ownerName));
	CircleCollider* pCollider = uCollider.get();
	m_colliders.emplace_back(std::move(uCollider));
	return pCollider;
}

AABBCollider* Physics::CreateAABBCollider(std::string ownerName)
{
	auto uCollider = std::make_unique<AABBCollider>(std::move(ownerName));
	AABBCollider* pCollider = uCollider.get();
	m_colliders.emplace_back(std::move(uCollider));
	return pCollider;
}

void Physics::Scrub()
{
	Core::RemoveIf<std::unique_ptr<Collider>>(m_colliders, [](std::unique_ptr<Collider>& uCollider) { return uCollider->m_bDestroyed; });
}
} // namespace ME
