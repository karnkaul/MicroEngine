#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "ViewportData.h"
#include "GameTypes.h"

namespace ME
{
// \brief: Derives from sf::RenderWindow
// Implementation provides SFML libs for linker via pragma
class Viewport final : public sf::RenderWindow
{
private:
	ViewportData m_data;

public:
	static ViewportSize MaxSize();

public:
	Viewport();
	~Viewport() override;

	void SetData(ViewportData data);
	void Create(u8 AAlevel, u32 framerateLimit = 300);
	void Destroy();
	void OverrideData(ViewportRecreateData data);
};
} // namespace ME
