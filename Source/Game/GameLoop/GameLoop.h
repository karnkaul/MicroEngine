#pragma once
#pragma once
#include "../GameTypes.h"

namespace ME
{
namespace GameLoop
{
s32 Run(u8 minFPS, u8 maxFPS, s32 argc, char** argv);
void Stop();

const std::string_view PWD();
} // namespace GameLoop
} // namespace ME
