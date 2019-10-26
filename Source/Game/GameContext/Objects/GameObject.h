#pragma once
#include <bitset>
#include "Engine/Rendering/Primitive.h"
#include "Collision.h"
#include "../GameObjectBase.h"

namespace ME
{
class GameObject : public GameObjectBase
{
protected:
	enum class Flags
	{
		Enabled = 0,
		Destroyed,
		Despawned,
	};

public:
	static const Colour DEFAULT_TEXT_COLOUR;
	static const u32 DEFAULT_TEXT_SIZE;

public:
	Transform m_transform;
	LayerID m_layer = 0;

protected:
	Collision m_collision;
	HPrim m_hPrim = INVALID_HANDLE;
	Primitive* m_pPrim = nullptr;
	std::bitset<3> m_flags;

private:
	TextData m_textData;
	class ObjectPool* m_pPool = nullptr;

public:
	GameObject();
	GameObject(GameObject&&);
	~GameObject() override;

public:
	Primitive& GetPrim();
	GameObject& Instantiate(Primitive::Type type);
	GameObject& SetText(const TextData& data);
	GameObject& SetShape(const ShapeData& data);
	GameObject& SetSprite(const SpriteData& data);

	const Collision& GetCollision() const;
	Collision& GetCollision();

	virtual void SetEnabled(bool bEnabled);
	void Destroy();

	bool IsEnabled() const;
	bool IsDestroyed() const;
	Rect2 Bounds() const;

protected:
	virtual void OnCreate();
	virtual void OnRespawn();
	virtual void Tick(Time dt);

private:
	void Create(std::string name);
	void Respawn(std::string name);

	friend class GameWorld;
	friend class Collision;
	friend class ObjectPool;
};
} // namespace ME
