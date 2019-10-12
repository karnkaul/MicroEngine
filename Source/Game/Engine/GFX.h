#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "GameTypes.h"
#include "Engine/Viewport/ViewportData.h"

namespace ME
{
class GFX final
{
private:
	std::map<u32, ViewportSize> m_viewportSizes;
	ViewportSize m_maxViewportSize;
	ViewportSize m_viewportSize;
	sf::FloatRect m_uiViewCrop;
	Vector2 m_letterBoxInverse;
	Vector2 m_worldSpace;
	Vector2 m_overlaySpace;
	f64 m_nativeAspectRatio;
	f64 m_uiAspectRatio;

public:
	// Set by game
	Vector2 m_uiSpace;
	Fixed m_worldHeight;
	Fixed m_viewportHeight;
	f64 m_overrideNativeAR = 0.0;

public:
	void Init();

	const std::map<u32, ViewportSize>& ValidViewportSizes() const;
	const ViewportSize* MaxViewportSize(bool bBorderless) const;
	const ViewportSize* TryGetViewportSize(u32 height) const;

	const ViewportSize& GetViewportSize() const;
	const Vector2& UISpace() const;
	const sf::FloatRect& UIViewCrop() const;
	Fixed WorldHeight() const;
	const Vector2& WorldSpace() const;
	const Vector2& OverlaySpace() const;
	f64 NativeAspectRatio() const;
	f64 UIAspectRatio() const;

	Vector2 NormalisedPosition(Vector2 position, Vector2 size) const;
	Vector2 Projection(Vector2 nPos, Vector2 size) const;
	Vector2 UIProjection(Vector2 nPos) const;
	Vector2 WorldProjection(Vector2 nPos) const;
	Vector2 OverlayProjection(Vector2 nPos) const;
	Vector2 WorldToUI(Vector2 world) const;
	Vector2 WorldToOverlay(Vector2 world) const;
	Vector2 ViewportToWorld(s32 vpX, s32 vpY) const;
	Vector2 WorldToViewport(Vector2 world) const;
};
} // namespace ME
