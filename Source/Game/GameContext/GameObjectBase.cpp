#include "Engine/GameServices.h"
#include "GameObjectBase.h"

namespace ME
{
GameObjectBase::GameObjectBase() = default;
GameObjectBase::GameObjectBase(GameObjectBase&&) = default;
GameObjectBase& GameObjectBase::operator=(GameObjectBase&&) = default;
GameObjectBase::~GameObjectBase() = default;

void GameObjectBase::SetHandle(HObj handle)
{
	m_handle = handle;
}

std::string_view GameObjectBase::Name() const
{
	return m_name;
}

std::string_view GameObjectBase::Type() const
{
	return m_type.empty() ? (m_type = ME::Typename(*this)) : m_type;
}

void GameObjectBase::RegisterInput(std::function<bool(const Input::Frame& frame)> callback, bool bForceEveryFrame)
{
	Assert(g_pInput, "Input is null!");
	m_inputTokens.push_back(g_pInput->Register(callback, bForceEveryFrame));
}
} // namespace ME
