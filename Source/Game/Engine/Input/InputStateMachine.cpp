#include "InputStateMachine.h"
#include "../SFTypes.h"

namespace ME
{
namespace
{
const Dual<u8> VALID_RANGE = {5, 127};
std::unordered_map<u32, KeyCode> ignoreMap = {
	{8, KeyCode::Backspace}, {9, KeyCode::Tab}, {13, KeyCode::Enter}, {27, KeyCode::Escape}, {15, KeyCode::LShift}, {16, KeyCode::RShift},
};
} // namespace

InputStateMachine::InputStateMachine()
{
	// Pre-defined keys
	m_keyStateMap = {
		{KeyCode::Left, {KeyCode::Left, "Left"}},
		{KeyCode::Right, {KeyCode::Right, "Right"}},
		{KeyCode::Up, {KeyCode::Up, "Up"}},
		{KeyCode::Down, {KeyCode::Down, "Down"}},
		{KeyCode::W, {KeyCode::W, "W"}},
		{KeyCode::A, {KeyCode::A, "A"}},
		{KeyCode::S, {KeyCode::S, "S"}},
		{KeyCode::D, {KeyCode::D, "D"}},
		{KeyCode::Space, {KeyCode::Space, "Space"}},
		{KeyCode::Enter, {KeyCode::Enter, "Enter"}},
		{KeyCode::Escape, {KeyCode::Escape, "Escape"}},
		{KeyCode::Tab, {KeyCode::Tab, "Tab"}},
		{KeyCode::Tilde, {KeyCode::Tilde, "Backtick"}},
		{KeyCode::Backspace, {KeyCode::Backspace, "Backspace"}},
		{KeyCode::Delete, {KeyCode::Delete, "Delete"}},

		{KeyType::MOUSE_BTN_0, {KeyType::MOUSE_BTN_0, "Mouse0"}},
		{KeyType::MOUSE_BTN_1, {KeyType::MOUSE_BTN_1, "Mouse1"}},
		{KeyType::MOUSE_BTN_2, {KeyType::MOUSE_BTN_2, "Mouse2"}},
		{KeyType::MOUSE_BTN_3, {KeyType::MOUSE_BTN_3, "Mouse3"}},
		{KeyType::MOUSE_BTN_4, {KeyType::MOUSE_BTN_4, "Mouse4"}},
	};
	m_pointerInput.bInViewport = true;
}

InputStateMachine::~InputStateMachine() = default;

InputDataFrame InputStateMachine::Dataframe() const
{
	InputDataFrame frame;
	frame.textInput = m_textInput;
	for (const auto& kvp : m_keyStateMap)
	{
		const auto& keyState = kvp.second;
		if (keyState.bPressed)
		{
			frame.pressed.push_back(keyState);
		}
	}
	frame.mouseInput = m_pointerInput;
	return frame;
}

KeyState& InputStateMachine::GetOrCreateKeyState(KeyType key)
{
	if (m_keyStateMap.find(key) == m_keyStateMap.end())
	{
		m_keyStateMap.emplace(key, KeyState(key));
	}
	return m_keyStateMap.at(key);
}

void InputStateMachine::ModifyKeyState(KeyType key, bool bPressed)
{
	if (key != KeyType::KEYTYPE_INVALID)
	{
		KeyState& toModify = GetOrCreateKeyState(key);
		toModify.bPressed = bPressed;
	}
}

void InputStateMachine::OnKeyDown(const sf::Event::KeyEvent& key)
{
	KeyType keyType = static_cast<KeyType>(key.code);
	ModifyKeyState(keyType, true);
	if (keyType != KeyType::KEYTYPE_INVALID)
	{
		m_textInput.metaText.emplace(keyType);
	}
}

void InputStateMachine::OnKeyUp(const sf::Event::KeyEvent& key)
{
	ModifyKeyState(static_cast<KeyType>(key.code), false);
}

void InputStateMachine::OnMouseDown(const sf::Event::MouseButtonEvent& button)
{
	ModifyKeyState(Cast(button.button), true);
}

void InputStateMachine::OnMouseUp(const sf::Event::MouseButtonEvent& button)
{
	ModifyKeyState(Cast(button.button), false);
}

void InputStateMachine::OnMouseMove(Vector2 worldPosition)
{
	m_pointerInput.worldPosition = worldPosition;
}

void InputStateMachine::OnMouseChange(bool bInViewport)
{
	m_pointerInput.bInViewport = bInViewport;
}

void InputStateMachine::SetMouseWheelScroll(Fixed delta)
{
	m_pointerInput.scrollDelta = delta;
}

void InputStateMachine::ResetKeyStates()
{
	for (auto& kvp : m_keyStateMap)
	{
		kvp.second.bPressed = false;
	}
	m_pointerInput.scrollDelta = Fixed::Zero;
}

void InputStateMachine::ClearTextInput()
{
	m_textInput.Reset();
}

void InputStateMachine::OnTextInput(u32 unicode)
{
	if (unicode > VALID_RANGE.first && unicode < VALID_RANGE.second)
	{
		auto iter = ignoreMap.find(unicode);
		if (iter == ignoreMap.end())
		{
			m_textInput.text += static_cast<char>(unicode);
		}
		else
		{
			m_textInput.metaText.emplace(static_cast<KeyType>(iter->second));
		}
	}
}
} // namespace ME
