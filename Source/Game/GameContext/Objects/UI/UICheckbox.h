#pragma once
#include "UIWidget.h"

namespace ME
{
class UICheckbox : public UIWidget
{
protected:
	bool isChecked = false;

protected:
	void OnCreate() override;

protected:
	void Select() override;
	void Deselect() override;
	void InteractBegin() override;
	void InteractEnd() override;

public:
	void ToggleChecked();
};
} // namespace ME
