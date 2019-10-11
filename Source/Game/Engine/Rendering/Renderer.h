#pragma once
#include <optional>
#include "Factory/Factory.h"
#include "GameTypes.h"
#include "Colour.h"

namespace sf
{
class Drawable;
class Transformable;
} // namespace sf

namespace ME
{
using LayerID = s32;
using HPrim = s32;

struct TextData
{
	std::optional<std::string> oText;
	std::optional<u32> oCharSize;
	std::optional<class Font*> opFont;

	TextData();
	TextData(const char* szText);
	TextData(std::string text);
};

struct ShapeData
{
	std::optional<Vector2> oSize;
	std::optional<Colour> oFill;
	std::optional<Colour> oOutline;
};

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
	LayerID m_layer = 0;

private:
	std::unique_ptr<sf::Drawable> m_uD;
	sf::Transformable* m_pT = nullptr;
	Matrix3 m_matrix;
	
public:
	Primitive();
	~Primitive();
	Primitive(Primitive&&);

public:
	Primitive* Instantiate(Type type);
	Primitive* SetText(const TextData& data);
	Primitive* SetShape(const ShapeData& data);
	Primitive* SetPosition(Vector2 world);

public:
	template <typename T>
	T* CastDrawable();

private:
	void Draw(class Viewport& viewport);

	friend class Renderer;
};

class Renderer final : NoCopy
{
private:
	Factory<Primitive> m_factory;

public:
	Renderer();
	~Renderer();

public:
	void Clear();

	HPrim New();
	Primitive* Find(HPrim handle);

	void Render(Viewport& viewport);
};

template <typename T>
T* Primitive::CastDrawable()
{
	static_assert(std::is_base_of<sf::Drawable, T>::value, "T must derive from sf::Drawable!");
	return dynamic_cast<T*>(m_uD.get());
}
} // namespace ME
