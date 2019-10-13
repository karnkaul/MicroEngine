#pragma once
#include "../GameObjectBase.h"
#include "Engine/Rendering/Primitive.h"

namespace ME
{
class GameObject : public GameObjectBase
{
public:
	static const Colour DEFAULT_TEXT_COLOUR;
	static const u32 DEFAULT_TEXT_SIZE;

public:
	Transform m_transform;
	LayerID m_layer;
	bool m_bEnabled = true;

protected:
	HPrim m_hPrim = INVALID_HANDLE;
	Primitive* m_pPrim = nullptr;

private:
	TextData m_textData;
	bool m_bDestroyed = false;

public:
	GameObject();
	GameObject(GameObject&&);
	~GameObject() override;

public:
	Primitive& GetPrim();
	GameObject& Instantiate(Primitive::Type type);
	GameObject& SetText(const TextData& data);
	GameObject& SetShape(const ShapeData& data);
	void Destroy();

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
