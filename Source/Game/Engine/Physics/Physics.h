#pragma once
#include "GameTypes.h"

namespace ME
{
class Physics final
{
private:
	std::vector<std::unique_ptr<class Collider>> m_colliders;
	std::string m_logName;

public:
	Physics();
	~Physics();

	class CircleCollider* CreateCircleCollider(std::string ownerName);
	class AABBCollider* CreateAABBCollider(std::string ownerName);

private:
	void Clear();
	void Update();

	void Scrub();

private:
	friend class GameWorld;
};
} // namespace ME
