#include "UICheckbox.h"

namespace ME
{
void UICheckbox::OnCreate()
{
	UIWidget::OnCreate();
	Instantiate(Primitive::Type::Rectangle);
	ShapeData data;
	data.oOutline = Colour::White;
	data.oSize = {25, 25};
	data.oBorder = 3;
	data.oFill = s_defaultColours.deselected;
	SetShape(data);
}

void UICheckbox::Select()
{
	ShapeData data;
	data.oFill = s_defaultColours.selected;
	SetShape(data);
}

void UICheckbox::Deselect()
{
	ShapeData data;
	if (GetChecked())
	{
		data.oFill = s_defaultColours.clicked;
	}
	else
	{
		data.oFill = s_defaultColours.deselected;
	}
	SetShape(data);
}

void UICheckbox::InteractBegin()
{
	SetChecked(!m_bIsChecked);
}

void UICheckbox::InteractEnd()
{
	m_onInteract();
}

void UICheckbox::SetChecked(bool bChecked)
{
	ShapeData data;
	if (bChecked)
	{
		data.oFill = s_defaultColours.clicked;
	}
	else
	{
		data.oFill = s_defaultColours.deselected;
	}
	SetShape(data);
	m_bIsChecked = bChecked;
}

bool UICheckbox::GetChecked()
{
	return m_bIsChecked;
}
} // namespace ME
