#include <fstream>
#include <string>
#include <unordered_map>
#include "GameSettings.h"
#include "GameLoop.h"
#include "OS.h"

namespace ME
{
using Props = std::unordered_map<std::string, std::string>;

namespace
{
bool IsDirtyAndSet(const Props& props, s32& toSet, const std::string& key)
{
	auto iter = props.find(key);
	if (iter != props.end())
	{
		s32 val = Strings::ToS32(iter->second, toSet);
		if (val != toSet)
		{
			toSet = val;
			return true;
		}
		return false;
	}
	return true;
}

bool IsDirtyAndSet(const Props& props, bool& bToSet, const std::string& key)
{
	auto iter = props.find(key);
	if (iter != props.end())
	{
		bool bVal = Strings::ToBool(iter->second, bToSet);
		if (bVal != bToSet)
		{
			bToSet = bVal;
			return true;
		}
		return false;
	}
	return true;
}
} // namespace

std::string GameSettings::s_filePath = "Settings.txt";

GameSettings& GameSettings::Inst()
{
	static GameSettings settings;
	return settings;
}

GameSettings::GameSettings()
{
	Load();
	if (m_bAutosave && m_bDirty)
	{
		Save();
	}
}

GameSettings::~GameSettings()
{
	if (m_bAutosave && m_bDirty)
	{
		Save();
	}
}

void GameSettings::Load()
{
	Props properties;
	std::ifstream file(OS::FullPath(s_filePath));
	if (file.good())
	{
		std::string line;
		while (std::getline(file, line))
		{
			auto tokens = Strings::Tokenise(line, '=', {});
			if (tokens.size() > 1)
			{
				Strings::ToLower(tokens.at(0));
				properties.emplace(std::move(tokens.at(0)), std::move(tokens.at(1)));
			}
		}
	}
	m_bDirty |= IsDirtyAndSet(properties, m_aaLevel, "aa_level");
	m_bDirty |= IsDirtyAndSet(properties, m_bVsync, "vsync");
	m_bDirty |= IsDirtyAndSet(properties, m_maxFPS, "max_fps");
	m_bDirty |= IsDirtyAndSet(properties, m_logSeverity, "log_level");
	// Verify
	m_aaLevel = Maths::Clamp(m_aaLevel, 0, 16);
	m_maxFPS = Maths::Clamp(m_maxFPS, 0, 500);
	m_logSeverity = Maths::Clamp(m_logSeverity, 0, ToS32(LE::LogSeverity::Error));
}

void GameSettings::Save()
{
	std::ofstream file(OS::FullPath(s_filePath));
	if (file.good())
	{
		std::string text;
		text.reserve(1024);
		text += "AA_LEVEL=";
		text += std::to_string(m_aaLevel);
		text += "\nVSYNC=";
		text += Strings::ToString(m_bVsync);
		text += "\nMAX_FPS=";
		text += std::to_string(m_maxFPS);
		text += "\nLOG_LEVEL=";
		text += std::to_string(m_logSeverity);
		text += "\n";
		file << text;
		// file.write(text.data(), static_cast<std::streamsize>(text.size()));
	}
	m_bDirty = false;
}
} // namespace ME
