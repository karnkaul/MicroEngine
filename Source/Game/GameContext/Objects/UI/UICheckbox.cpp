#include "UICheckbox.h"

namespace ME
{
void UICheckbox::OnCreate()
{
	UIWidget::OnCreate();
	Instantiate(Primitive::Type::Rectangle);
	{
		ShapeData data;
		data.oOutline = Colour::White;
		data.oSize = {25, 25};
		data.oBorder = 3;
		data.oFill = Colour(200, 225, 255);
		SetShape(data);
	}
}

void UICheckbox::Select() {}

void UICheckbox::Deselect(){}

void UICheckbox::InteractBegin()
{
	ToggleChecked();
}

void UICheckbox::InteractEnd()
{
	m_onInteract();
}

void UICheckbox::ToggleChecked()
{
	ShapeData data;
	if (isChecked)
	{
		data.oFill = Colour(200, 225, 255);
	}
	else
	{
		data.oFill = Colour(100, 125, 155);
	}
	SetShape(data);
	isChecked = !isChecked;
}
} // namespace ME
