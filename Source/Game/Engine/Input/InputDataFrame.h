#pragma once
#include <string>
#include <unordered_set>
#include "GameTypes.h"
#include "InputMappings.h"

namespace ME
{
struct KeyState
{
private:
	s32 keyType;
	std::string_view name;

public:
	bool bPressed;

public:
	KeyState(s32 keyType, std::string_view name = "Unknown");

	KeyType GetKeyType() const;
	std::string_view Name() const;
};

struct TextInput
{
	std::string text;
	std::unordered_set<KeyType> metaText;

	bool ContainsChar(char c) const;
	bool ContainsKey(s32 keyCode) const;
	void Reset();
};

struct MouseInput
{
	Vector2 worldPosition;
	Fixed scrollDelta;
	bool bInViewport = false;
};

struct InputDataFrame
{
	std::vector<KeyState> pressed;
	TextInput textInput;
	MouseInput mouseInput;

	static std::string Clipboard();
};
} // namespace ME
