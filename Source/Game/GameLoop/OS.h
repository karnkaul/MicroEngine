#pragma once
#include "GameTypes.h"

namespace ME
{
namespace OS
{
void Init(s32 argc, char** argv);
std::string_view PWD();
std::string FullPath(const std::string& subPath);
} // namespace OS
} // namespace ME
