#pragma once
#include "SFML/Window/Event.hpp"
#include "GameTypes.h"
#include "InputDataFrame.h"

namespace ME
{
// \brief Concrete class that a Graphics can update KeyStates to every frame
class InputStateMachine
{
private:
	TextInput m_textInput;
	MouseInput m_pointerInput;
	std::unordered_map<s32, KeyState> m_keyStateMap;

public:
	InputStateMachine();
	~InputStateMachine();

	InputDataFrame Dataframe() const;

private:
	KeyState& GetOrCreateKeyState(KeyType key);
	void ModifyKeyState(KeyType key, bool bPressed);

	void OnKeyDown(const sf::Event::KeyEvent& key);
	void OnKeyUp(const sf::Event::KeyEvent& key);
	void OnMouseDown(const sf::Event::MouseButtonEvent& button);
	void OnMouseUp(const sf::Event::MouseButtonEvent& button);
	void OnMouseMove(Vector2 worldPosition);
	void OnMouseChange(bool bInViewport);
	void SetMouseWheelScroll(Fixed delta);

	void ResetKeyStates();
	void ClearTextInput();
	void OnTextInput(u32 unicode);

	friend class InputHandler;
};
} // namespace ME
