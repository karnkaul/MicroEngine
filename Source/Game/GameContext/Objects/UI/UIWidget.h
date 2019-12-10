#pragma once
#include "Delegate/Delegate.h"
#include "../GameObject.h"

namespace ME
{
class UIWidget : public GameObject
{
public:
	using OnInteract = LE::Delegate<>;

public:
	enum class State
	{
		Deselected = 0,
		Selected,
		Interacting,
		UnInteractable
	};

	struct Colours
	{
		Colour deselected = Colour(200, 225, 255);
		Colour selected = Colour(150, 175, 200);
		Colour clicked = Colour(100, 125, 155);
	};
	static const Colours s_defaultColours;

public:
	Vector2 textPad = Vector2(20, 20);

private:
	State m_state;

protected:
	OnInteract m_onInteract;
	HObj m_hText;

protected:
	void OnCreate() override;
	void OnDestroy() override;

protected:
	virtual void Select() = 0;
	virtual void Deselect() = 0;
	virtual void InteractBegin() = 0;
	virtual void InteractEnd() = 0;

public:
	void SetEnabled(bool bEnabled) override;
	void SetInteractable(bool bInteractable);

public:
	UIWidget& SetUIText(const TextData& data);
	OnInteract::Token Register(OnInteract::Callback callback);

	State GetState() const;
};
} // namespace ME
