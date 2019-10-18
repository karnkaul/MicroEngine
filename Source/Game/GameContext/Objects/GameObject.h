#pragma once
#include <bitset>
#include "../GameObjectBase.h"
#include "Engine/Rendering/Primitive.h"

namespace ME
{
class GameObject : public GameObjectBase
{
protected:
	enum class Flags
	{
		Enabled = 0,
		Destroyed
	};

public:
	static const Colour DEFAULT_TEXT_COLOUR;
	static const u32 DEFAULT_TEXT_SIZE;

public:
	Transform m_transform;
	LayerID m_layer = 0;

protected:
	std::bitset<2> m_flags;

protected:
	HPrim m_hPrim = INVALID_HANDLE;
	Primitive* m_pPrim = nullptr;

private:
	TextData m_textData;

public:
	GameObject();
	GameObject(GameObject&&);
	~GameObject() override;

public:
	Primitive& GetPrim();
	GameObject& Instantiate(Primitive::Type type);
	GameObject& SetText(const TextData& data);
	GameObject& SetShape(const ShapeData& data);

	virtual void SetEnabled(bool bEnabled);
	void Destroy();

	bool IsEnabled() const;
	bool IsDestroyed() const;
	Rect2 Bounds() const;

protected:
	virtual void OnCreate();
	virtual void Tick(Time dt);

private:
	void Create(std::string id);

	friend class GameWorld;
};
} // namespace ME
