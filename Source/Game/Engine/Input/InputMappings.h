#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "GameTypes.h"

namespace ME
{
using KeyCode = sf::Keyboard::Key;

enum KeyType : s32
{
	KEYTYPE_INVALID = -1,
	// 0-499 : SFML

	MOUSE_BTN_0 = 500, // Left
	MOUSE_BTN_1,	   // Right
	MOUSE_BTN_2,	   // Middle
	MOUSE_BTN_3,
	MOUSE_BTN_4,
};
} // namespace ME
