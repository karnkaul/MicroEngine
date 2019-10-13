#pragma once
#include <functional>
#include <string>
#include <vector>
#include "GameTypes.h"
#include "Engine/Handles.h"
#include "Engine/Input/Input.h"

namespace ME
{
class GameObjectBase
{
protected:
	std::string m_name;
	mutable std::string_view m_type;
	std::vector<Token> m_inputTokens;

public:
	GameObjectBase();
	GameObjectBase(GameObjectBase&&);
	GameObjectBase& operator=(GameObjectBase&&);
	virtual ~GameObjectBase();

public:
	std::string_view Name() const;
	std::string_view Type() const;

protected:
	void RegisterInput(std::function<bool(const Input::Frame& frame)> callback, bool bForceEveryFrame = false);

	friend class GameContext;
};
} // namespace ME
