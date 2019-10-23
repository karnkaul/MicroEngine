#pragma once
#include "GameTypes.h"
#include "Engine/Resources/Resource.h"
#include "PrimitiveData.h"

namespace ME
{
// Maintains any number of texture coordinates to be applied on a sprite
class Spritesheet
{
private:
	Time m_elapsed;

public:
	std::vector<TexCoords> m_texCoords;
	size_t m_index = 0;
	Time m_period;
	Texture* m_pTexture = nullptr;
	
	void Autobuild(Texture& texture, u16 columns, u16 rows);
	// Returns true if m_index changes
	bool Tick(Time dt);
	void Reset(bool bClearTexCoords);
};
} // namespace ME
