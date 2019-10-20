#include "Game/GameLoop/GameLoop.h"

constexpr u8 MIN_FPS = 30;
constexpr u8 MAX_FPS = 120;

s32 main(s32 argc, char** argv)
{
	return ME::GameLoop::Run(MIN_FPS, MAX_FPS, argc, argv);
}
