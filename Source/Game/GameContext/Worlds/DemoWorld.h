#include "GameWorld.h"

namespace ME
{
class DemoWorld final : public GameWorld
{
private:
	HRes m_hSerifFont = INVALID_HANDLE;
	HPrim m_hPrim0 = INVALID_HANDLE;
	HPrim m_hPrim1 = INVALID_HANDLE;
	Time m_prim1LayerTTL;
	bool m_bPrim1LayerChanged = false;
	Time m_prim1TTL;
	bool m_bPrim1Destroyed = false;

protected:
	void OnCreated() override;
	void Start() override;
	void Tick(Time dt) override;
	void Stop() override;
};
}
