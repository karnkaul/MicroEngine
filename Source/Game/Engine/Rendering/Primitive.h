#pragma once
#include "PrimitiveData.h"

namespace sf
{
class Drawable;
class Transformable;
} // namespace sf

namespace ME
{
class Primitive final
{
public:
	enum class Type
	{
		Rectangle = 0,
		Circle,
		Text,
		Sprite,
	};

public:
	Transform m_transform;
	LayerID m_layer = 0;

private:
	std::unique_ptr<sf::Drawable> m_uD;
	sf::Transformable* m_pT = nullptr;
	s32 m_handle = INVALID_HANDLE;

public:
	Primitive();
	Primitive(Primitive&&);
	~Primitive();
	
public:
	Primitive* Instantiate(Type type);
	Primitive* SetText(const TextData& data);
	Primitive* SetShape(const ShapeData& data);

public:
	template <typename T>
	T* CastDrawable();

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
} // namespace ME
