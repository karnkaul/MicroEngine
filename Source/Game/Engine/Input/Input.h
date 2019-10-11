#pragma once
#include <vector>
#include <optional>
#include "InputHandler.h"
#include "InputDataFrame.h"

namespace ME
{
using namespace std::placeholders;
namespace Debug
{
class ConsoleInput;
}

class Input final : public InputHandler
{
public:
	struct Frame
	{
		using GameInputs = std::vector<KeyType>;

		std::vector<KeyType> pressed;
		std::vector<KeyType> held;
		std::vector<KeyType> released;
		TextInput textInput;
		MouseInput mouseInput;

		static std::string Clipboard();

		bool IsPressed(s32 key) const;
		bool IsHeld(s32 key) const;
		bool IsReleased(s32 key) const;

		bool IsPressed(std::initializer_list<s32> keys, bool bAny = true) const;
		bool IsHeld(std::initializer_list<s32> keys, bool bAny = true) const;
		bool IsReleased(std::initializer_list<s32> keys, bool bAny = true) const;

		bool HasData() const;

		Fixed MouseWhellScroll() const;

	private:
		inline bool Result(std::initializer_list<s32> keys, std::function<bool(s32)> subroutine, bool bAny) const
		{
			bool bResult = bAny ? false : true;
			for (auto key : keys)
			{
				if (bAny)
				{
					bResult |= subroutine(key);
				}
				else
				{
					bResult &= subroutine(key);
				}
			}
			return bResult;
		}
	};

	using Delegate = std::function<bool(const Frame& frameData)>;

private:
	struct InputContext
	{
		Delegate callback;
		WToken wToken;
		bool bForce = false;
	};

private:
	std::vector<KeyType> m_previousSnapshot;
	std::vector<KeyType> m_currentSnapshot;
	TextInput m_textInput;
	MouseInput m_mouseInput;
	std::vector<InputContext> m_contexts;
	std::optional<InputContext> m_oSudoContext;
	mutable s32 m_nextToken = 0;

public:
	Token Register(Delegate callback, bool bForce = false);
	MouseInput MouseState() const;

	Token RegisterSudo(Delegate callback);
	void TakeSnapshot();
	void FireCallbacks();

	Token CreateToken() const;
};
} // namespace LittleEngine
