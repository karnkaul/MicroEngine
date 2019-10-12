#pragma once
#include <functional>
#include <mutex>
#include <typeinfo>
#include <utility>
#include "Delegate/Delegate.h"
#include "GameTypes/Fixed.h"
#include "GameTypes/Matrix3.h"
#include "GameTypes/Rect2.h"
#include "GameTypes/SimpleTime.h"
#include "GameTypes/Transform.h"
#include "GameTypes/UByte.h"
#include "GameTypes/Utils.h"
#include "GameTypes/Vector2.h"
#include "Factory/Factory.h"
#include "NoCopy.h"

#define ENABLED(x) x

#if _MSC_VER
#define CXX_MS_CRT
#elif __linux__ || __MINGW32__
#define CXX_LIBC
#else
#define CXX_UNDEFINED
#endif

#if _WIN64
#define TARGET_WIN64
#elif __linux__
#define TARGET_LINUX
#else
#define TARGET_UNDEFINED
#endif

#if _MSC_VER
#define STD_FILESYSTEM 1
#else
#define STD_FILESYSTEM 0
#endif

#if defined(DEBUGGING)
#define DEBUG_LOGGING 1
#define ASSERTS 1
#else
#define DEBUG_LOGGING 0
#define ASSERTS 0
#endif

#include "Asserts/Asserts.h"
#include "Logger/Logger.h"

namespace ME
{
constexpr s32 INVALID_HANDLE = -1;
using LayerID = s32;

using Lock = std::lock_guard<std::mutex>;
using Task = std::function<void()>;

template <typename T>
using UPtr = std::unique_ptr<T>;
template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using WPtr = std::weak_ptr<T>;
template <typename T>
using Dual = std::pair<T, T>;
template <typename T, typename U>
using Pair = std::pair<T, U>;

using Token = SPtr<s32>;
using WToken = WPtr<s32>;

using Fixed = Core::Fixed;
using Matrix3 = Core::Matrix3;
using Rect2 = Core::Rect2;
using Transform = Core::Transform;
using Time = Core::Time;
using UByte = Core::UByte;
using Vector2 = Core::Vector2;
using Vector3 = Core::Vector3;

template <typename T>
Token MakeToken(T value)
{
	return std::make_shared<s32>(ToS32(value));
}

template <typename T>
std::string_view Typename(const T& t)
{
	static const std::string_view PREFIX = "class ME::";
	static const size_t PREFIX_LEN = PREFIX.length();
	std::string_view name = typeid(t).name();
	auto idx = name.find(PREFIX);
	if (idx != std::string::npos)
	{
		name = name.substr(PREFIX_LEN, name.length() - PREFIX_LEN);
	}
	return name;
}

template <typename T>
std::string_view Typename()
{
	static const std::string_view PREFIX = "class ME::";
	static const size_t PREFIX_LEN = PREFIX.length();
	std::string_view name = typeid(T).name();
	auto idx = name.find(PREFIX);
	if (idx != std::string::npos)
	{
		name = name.substr(PREFIX_LEN, name.length() - PREFIX_LEN);
	}
	return name;
}
} // namespace ME
