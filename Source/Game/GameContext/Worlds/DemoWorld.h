#include "GameWorld.h"

namespace ME
{
class DemoWorld final : public GameWorld
{
private:
	HRes m_hSerifFont = INVALID_HANDLE;
	HObj m_hObj0 = INVALID_HANDLE;
	HObj m_hObj1 = INVALID_HANDLE;
	Time m_obj1LayerTTL;
	bool m_bObj1LayerChanged = false;
	Time m_obj1TTL;
	bool m_bObj1Destroyed = false;

protected:
	void OnCreated() override;
	void OnStarting() override;
	void Tick(Time dt) override;
	void OnStopping() override;
};
}
