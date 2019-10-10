#pragma once
#include "InputStateMachine.h"

namespace ME
{
enum class ViewportEventType : u8
{
	None = 0,
	Closed,
	LostFocus,
	GainedFocus,
	Resized
};

class InputHandler
{
protected:
	InputStateMachine m_inputSM;

public:
	InputHandler();
	~InputHandler();

public:
	[[nodiscard]] ViewportEventType PollEvents(class Viewport& sfWindow);
	InputDataFrame Dataframe() const;
};
} // namespace ME
