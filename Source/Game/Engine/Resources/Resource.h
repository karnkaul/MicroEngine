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

public:
	Font();
	~Font() override;

protected:
	bool OnLoad() override;
};
} // namespace ME
