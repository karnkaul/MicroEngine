#include "Engine/GameServices.h"
#include "GameObjectBase.h"

namespace ME
{
GameObjectBase::GameObjectBase() = default;
GameObjectBase::GameObjectBase(GameObjectBase&&) = default;
GameObjectBase::~GameObjectBase() 
{
	LOG_D("[%s] %s destroyed", m_name.data(), Type().data());
}

std::string_view GameObjectBase::Name() const
{
	return m_name;
}

std::string_view GameObjectBase::Type() const
{
	return m_type.empty() ? (m_type = ME::Typename(*this)) : m_type;
}

void GameObjectBase::RegisterInput(std::function<bool(const Input::Frame& frame)> callback)
{
	Assert(g_pInput, "Input is null!");
	m_inputTokens.push_back(g_pInput->Register(callback));
}
} // namespace ME
