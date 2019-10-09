#pragma once
#include <memory>
#include <type_traits>
#include <unordered_map>

namespace ME
{
template <typename FType, typename FHandle = int>
class Factory
{
protected:
	FHandle m_nextHandle = FHandle(0);
	std::unordered_map<FHandle, std::unique_ptr<FType>> m_instanced;

public:
	Factory();
	virtual ~Factory();

public:
	template <typename T>
	FHandle New();
	template <typename T>
	T* Find(FHandle h) const;
	bool Destroy(FHandle h);
};

template <typename FType, typename FHandle>
Factory<FType, FHandle>::Factory() = default;

template <typename FType, typename FHandle>
Factory<FType, FHandle>::~Factory()
{
	m_instanced.clear();
}

template <typename FType, typename FHandle>
template <typename T>
FHandle Factory<FType, FHandle>::New()
{
	static_assert(std::is_base_of<FType, T>::value, "T must derive from FType!");
	m_instanced.emplace(++m_nextHandle, std::make_unique<T>());
	return m_nextHandle;
}

template <typename FType, typename FHandle>
template <typename T>
T* Factory<FType, FHandle>::Find(FHandle h) const
{
	static_assert(std::is_base_of<FType, T>::value, "T must derive from FType!");
	auto search = m_instanced.find(h);
	if (search != m_instanced.end())
	{
		return dynamic_cast<T*>(search->second.get());
	}
	return nullptr;
}

template <typename FType, typename FHandle>
bool Factory<FType, FHandle>::Destroy(FHandle h)
{
	auto search = m_instanced.find(h);
	if (search != m_instanced.end())
	{
		m_instanced.erase(search);
		return true;
	}
	return false;
}
} // namespace ME