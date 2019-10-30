#include <string>
#include "OS.h"

namespace ME
{
namespace
{
std::string pwd;
}

void OS::Init(s32 argc, char** argv)
{
#if ENABLED(ASSERTS)
	std::string msg;
	msg.reserve(16);
	msg += "Invalid argc! ";
	msg += std::to_string(argc);
	AssertVar(argc > 0, msg.c_str());
#endif
	if (argc > 0)
	{
		std::string_view exePath = argv[0];
#ifdef TARGET_WIN64
		const char delim = '\\';
#else
		const char delim = '/';
#endif
		pwd = exePath.substr(0, exePath.find_last_of(delim));
		LOG_I("[OS] Working dir: %s", pwd.data());
	}
}

std::string_view OS::PWD()
{
	return pwd;
}

std::string OS::FullPath(const std::string& subPath)
{
	std::string ret;
	if (!pwd.empty())
	{
		ret += pwd;
		ret += "/";
	}
	ret += subPath;
	return ret;
}
} // namespace ME
