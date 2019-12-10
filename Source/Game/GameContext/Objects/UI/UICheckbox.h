#pragma once
#include "UIWidget.h"

namespace ME
{
class UICheckbox : public UIWidget
{
protected:
	bool m_bIsChecked = false;

protected:
	void OnCreate() override;

protected:
	void Select() override;
	void Deselect() override;
	void InteractBegin() override;
	void InteractEnd() override;

public:
	void SetChecked(bool bChecked);
	bool GetChecked();
};
} // namespace ME
