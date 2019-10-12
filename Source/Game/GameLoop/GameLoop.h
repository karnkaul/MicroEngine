#pragma once
#include "../GameTypes.h"

namespace ME
{
namespace GameLoop
{
s32 Run(s32 argc, char** argv);
void Stop();

const std::string_view PWD();
} // namespace GameLoop
} // namespace ME
