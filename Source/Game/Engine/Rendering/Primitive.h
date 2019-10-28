#pragma once
#include "PrimitiveData.h"
#include "Engine/Handles.h"
#include "Engine/Rendering/LayerIDs.h"

namespace sf
{
class Drawable;
class Transformable;
} // namespace sf

namespace ME
{
class Primitive final
{
#if defined(DEBUGGING)
public:
	bool m_bDEBUG = false;
#endif
public:
	enum class Type
	{
		Rectangle = 0,
		Circle,
		Text,
		Sprite,
	};

public:
	Vector2 m_position;
	Vector2 m_orientation = Vector2::Right;
	LayerID m_layer = Layers::L0100_DEFAULT;
	Type m_type;
	bool m_bEnabled = true;

private:
	std::unique_ptr<sf::Drawable> m_uD;
	sf::Transformable* m_pT = nullptr;
	HPrim m_handle = INVALID_HANDLE;

public:
	Primitive();
	Primitive(Primitive&&);
	Primitive& operator=(Primitive&&);
	~Primitive();

public:
	void SetHandle(HPrim handle);

public:
	Primitive* Instantiate(Type type);
	Primitive* SetText(const TextData& data);
	Primitive* SetShape(const ShapeData& data);
	Primitive* SetSprite(const SpriteData& data);

	Rect2 Bounds() const;

public:
	template <typename T>
	T* CastDrawable();
	template <typename T>
	const T* CastDrawable() const;

private:
	void Update();
	void Draw(class Viewport& viewport);

	friend class Renderer;
};

template <typename T>
T* Primitive::CastDrawable()
{
	static_assert(std::is_base_of<sf::Drawable, T>::value, "T must derive from sf::Drawable!");
	return dynamic_cast<T*>(m_uD.get());
}

template <typename T>
const T* Primitive::CastDrawable() const
{
	static_assert(std::is_base_of<sf::Drawable, T>::value, "T must derive from sf::Drawable!");
	return dynamic_cast<T*>(m_uD.get());
}
} // namespace ME
