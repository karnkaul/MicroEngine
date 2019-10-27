#pragma once
#include "UIWidget.h"

namespace ME
{
class UIButton : public UIWidget
{
public:
	Colour selectedText = Colour::Black;
	Colour deselectedText = Colour::Black;
	Colour interactingText = Colour::White;

protected:
	void OnCreate() override;

protected:
	void Select() override;
	void Deselect() override;
	void InteractBegin() override;
	void InteractEnd() override;
};
}
