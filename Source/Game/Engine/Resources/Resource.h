#pragma once
#include <string>
#include "SFML/Graphics/Font.hpp"
#include "GameTypes.h"

namespace ME
{
class Resource : NoCopy
{
protected:
	std::string m_id;
	std::string m_path;
	std::string_view m_type = "Unknown";
	bool m_bOK = false;

public:
	Resource();
	virtual ~Resource();

public:
	bool Load(std::string id, std::string path);

protected:
	virtual bool OnLoad() = 0;

private:
	friend class Resources;
};

class Font final : public Resource
{
public:
	sf::Font m_font;

protected:
	bool OnLoad() override;
};

class Text final : public Resource
{
public:
	std::string m_text;

protected:
	bool OnLoad() override;
};

class Texture final : public Resource
{
public:
	sf::Texture m_texture;

protected:
	bool OnLoad() override;
};
} // namespace ME
