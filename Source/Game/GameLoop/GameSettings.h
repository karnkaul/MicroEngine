#pragma once
#include "GameTypes.h"

namespace ME
{
class GameSettings final
{
public:
	static std::string s_filePath;

public:
	s32 m_aaLevel = 8;
	s32 m_maxFPS = 240;
	s32 m_logSeverity = 1;
	bool m_bVsync = true;
	bool m_bAutosave = true;

private:
	bool m_bDirty = false;

public:
	static GameSettings& Inst();

private:
	GameSettings();
	~GameSettings();

public:
	void Load();
	void Save();
};
} // namespace ME
