#pragma once
#include <vector>
#include "GameObject.h"

namespace ME
{
class Tilemap : public GameObject
{
private:
	std::vector<HPrim> m_prims;

protected:
	void OnRespawn() override;
	void OnDespawn() override;
	void OnDestroy() override;

public:
	void SetEnabled(bool bEnabled) override;

public:
	void FillSpace(Vector2 space, Texture& texture);
	void Clear();
};
} // namespace ME
