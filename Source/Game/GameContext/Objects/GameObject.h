#pragma once
#include "../GameObjectBase.h"
#include "Engine/Rendering/Primitive.h"

namespace ME
{
class GameObject : public GameObjectBase
{
protected:
	HPrim m_hPrim = INVALID_HANDLE;
	Primitive* m_pPrim = nullptr;

public:
	bool m_bEnabled = true;

public:
	GameObject();
	GameObject(GameObject&&);
	~GameObject() override;

public:
	Primitive& GetPrim();

protected:
	void RegisterInput(std::function<bool(const Input::Frame& frame)> callback);

protected:
	virtual void OnCreate();
	virtual void Tick(Time dt);

private:
	void Create(std::string id);

	friend class GameWorld;
};
} // namespace ME
