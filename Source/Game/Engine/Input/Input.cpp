#include "Input.h"

namespace ME
{
std::string Input::Frame::Clipboard()
{
	return InputDataFrame::Clipboard();
}

bool Input::Frame::IsPressed(s32 keyCode) const
{
	return Core::Search(pressed, static_cast<KeyType>(keyCode)) != pressed.end();
}

bool Input::Frame::IsHeld(s32 keyCode) const
{
	return Core::Search(held, static_cast<KeyType>(keyCode)) != held.end();
}

bool Input::Frame::IsReleased(s32 keyCode) const
{
	return Core::Search(released, static_cast<KeyType>(keyCode)) != released.end();
}

bool Input::Frame::IsPressed(std::initializer_list<s32> keys, bool bAny /* = true */) const
{
	return Result(
		keys, [&](s32 key) { return IsPressed(key); }, bAny);
}

bool Input::Frame::IsHeld(std::initializer_list<s32> keys, bool bAny /* = true */) const
{
	return Result(
		keys, [&](s32 key) { return IsHeld(key); }, bAny);
}

bool Input::Frame::IsReleased(std::initializer_list<s32> keys, bool bAny /* = true */) const
{
	return Result(
		keys, [&](s32 key) { return IsReleased(key); }, bAny);
}

bool Input::Frame::HasData() const
{
	return !pressed.empty() || !held.empty() || !released.empty();
}

Fixed Input::Frame::MouseWhellScroll() const
{
	return mouseInput.scrollDelta;
}

Input::Input() = default;
Input::~Input() = default;

Token Input::Register(Delegate callback, bool bForce)
{
	Token token = CreateToken();
	InputContext newTop{std::move(callback), token, bForce};
	m_contexts.emplace_back(newTop);
	return token;
}

MouseInput Input::MouseState() const
{
	return m_mouseInput;
}

Token Input::RegisterSudo(Delegate callback)
{
	Token token = CreateToken();
	InputContext sudo{std::move(callback), token, true};
	m_oSudoContext.emplace(sudo);
	return token;
}

void Input::TakeSnapshot()
{
	auto uniqueInsert = [&](s32 toInsert) {
		if (toInsert >= 0)
		{
			KeyType key = static_cast<KeyType>(toInsert);
			if (Core::Search(m_currentSnapshot, key) == m_currentSnapshot.end())
			{
				m_currentSnapshot.push_back(key);
			}
		}
	};
	auto frameData = m_inputSM.Dataframe();
	m_previousSnapshot = m_currentSnapshot;
	m_textInput = frameData.textInput;
	m_mouseInput = frameData.mouseInput;
	m_currentSnapshot.clear();
	for (const auto& key : frameData.pressed)
	{
		uniqueInsert(key.GetKeyType());
	}
}

void Input::FireCallbacks()
{
	std::vector<KeyType> pressed;
	std::vector<KeyType> held;
	std::vector<KeyType> released;

	// Build "pressed" and "held" vectors
	for (auto input : m_currentSnapshot)
	{
		auto search = Core::Search(m_previousSnapshot, input);
		if (search != m_previousSnapshot.end())
		{
			held.push_back(input);
		}
		else
		{
			pressed.push_back(input);
		}
	}

	released = m_previousSnapshot;
	Core::RemoveIf<KeyType>(released, [&held](KeyType type) { return Core::Search(held, type) != held.end(); });
	if (m_oSudoContext && m_oSudoContext->wToken.expired())
	{
		m_oSudoContext.reset();
	}

	Frame dataFrame{pressed, held, released, m_textInput, m_mouseInput};
	bool bHasData = dataFrame.HasData();
	size_t prev = m_contexts.size();
	Core::RemoveIf<InputContext>(m_contexts, [](const InputContext& context) { return context.wToken.expired(); });
	size_t curr = m_contexts.size();
	if (curr != prev)
	{
		LOG_D("[Input] Deleted [%d] stale registrants", prev - curr);
	}
	if (m_oSudoContext)
	{
		if (m_oSudoContext->callback(dataFrame))
		{
			return;
		}
	}

	for (auto iter = m_contexts.rbegin(); iter != m_contexts.rend(); ++iter)
	{
		if (bHasData || iter->bForce)
		{
			if (iter->callback(dataFrame))
			{
				return;
			}
		}
	}
}

Token Input::CreateToken() const
{
	return MakeToken(++m_nextToken);
}
} // namespace ME
