#pragma once
#include <memory>
#include <type_traits>
#include <unordered_map>

namespace ME
{
template <typename FType, typename FHandle = int>
class Factory
{
public:
	std::unordered_map<FHandle, FType> m_instanced;

protected:
	FHandle m_nextHandle = FHandle(0);

public:
	Factory();
	virtual ~Factory();

public:
	template <typename T>
	FHandle New();
	template <typename T>
	T* Find(FHandle h);
	bool Destroy(FHandle h);
	void Clear();
};

template <typename FType, typename FHandle = int>
class UFactory
{
public:
	std::unordered_map<FHandle, std::unique_ptr<FType>> m_instanced;

protected:
	FHandle m_nextHandle = FHandle(1);

public:
	UFactory();
	virtual ~UFactory();

public:
	template <typename T>
	FHandle New();
	template <typename T>
	T* Find(FHandle h) const;
	bool Destroy(FHandle h);
	void Clear();
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
	T t;
	m_instanced.emplace(m_nextHandle, std::move(t));
	return m_nextHandle++;
}

template <typename FType, typename FHandle>
template <typename T>
T* Factory<FType, FHandle>::Find(FHandle h)
{
	static_assert(std::is_base_of<FType, T>::value, "T must derive from FType!");
	auto search = m_instanced.find(h);
	if (search != m_instanced.end())
	{
		return dynamic_cast<T*>(&search->second);
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

template <typename FType, typename FHandle>
void Factory<FType, FHandle>::Clear()
{
	m_instanced.clear();
}

template <typename FType, typename FHandle>
UFactory<FType, FHandle>::UFactory() = default;

template <typename FType, typename FHandle>
UFactory<FType, FHandle>::~UFactory()
{
	m_instanced.clear();
}

template <typename FType, typename FHandle>
template <typename T>
FHandle UFactory<FType, FHandle>::New()
{
	static_assert(std::is_base_of<FType, T>::value, "T must derive from FType!");
	m_instanced.emplace(m_nextHandle, std::make_unique<T>());
	return m_nextHandle++;
}

template <typename FType, typename FHandle>
template <typename T>
T* UFactory<FType, FHandle>::Find(FHandle h) const
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
bool UFactory<FType, FHandle>::Destroy(FHandle h)
{
	auto search = m_instanced.find(h);
	if (search != m_instanced.end())
	{
		m_instanced.erase(search);
		return true;
	}
	return false;
}

template <typename FType, typename FHandle>
void UFactory<FType, FHandle>::Clear()
{
	m_instanced.clear();
}
} // namespace ME
