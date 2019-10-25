#pragma once
#include "Delegate/Delegate.h"
#include "GameTypes.h"
#include "Engine/Handles.h"
#include "Engine/Physics/Collider.h"

namespace ME
{
class Collision
{
public:
	struct Data
	{
		const class GameObject* pInstigator = nullptr;
		const Collider* pOther = nullptr;
	};
	using OnHit = LE::Delegate<Data>;

protected:
	struct Unit
	{
		Vector2 offset;
		class Collider* pCollider;
#if defined(DEBUGGING)
		HPrim hPrim = INVALID_HANDLE;
#endif
	};

	OnHit m_onHit;
	std::vector<Unit> m_colliders;
	class GameObject* m_pOwner;
	s32 m_signature = 0;

public:
	Collision(GameObject& owner);
	~Collision();

public:
	void Update(Vector2 position);

public:
	void SetEnabled(bool bEnabled);

public:
	OnHit::Token AddCircle(OnHit::Callback onHit, Fixed diameter, Vector2 offset = Vector2::Zero);
	OnHit::Token AddAABB(OnHit::Callback onHit, const AABBData& aabbData, Vector2 offset = Vector2::Zero);
	void RemoveAll();
};
} // namespace ME
