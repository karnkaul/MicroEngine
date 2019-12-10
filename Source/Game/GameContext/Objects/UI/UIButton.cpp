#include "UIButton.h"

namespace ME
{
void UIButton::OnCreate()
{
	UIWidget::OnCreate();
	Instantiate(Primitive::Type::Rectangle);
	{
		ShapeData data;
		data.oOutline = Colour::White;
		data.oSize = {75, 75};
		data.oFill = s_defaultColours.deselected;
		SetShape(data);
	}
	{
		TextData data;
		data.oFill = deselectedText;
		SetUIText(data);
	}
}

void UIButton::Select()
{
	{
		ShapeData data;
		data.oFill = s_defaultColours.selected;
		data.oBorder = 3;
		SetShape(data);
	}
	{
		TextData data;
		data.oFill = selectedText;
		SetUIText(data);
	}
}

void UIButton::Deselect()
{
	{
		ShapeData data;
		data.oFill = s_defaultColours.deselected;
		data.oBorder = 0;
		SetShape(data);
	}
	{
		TextData data;
		data.oFill = deselectedText;
		SetUIText(data);
	}
}

void UIButton::InteractBegin()
{
	{
		ShapeData data;
		data.oFill = s_defaultColours.clicked;
		data.oBorder = 3;
		SetShape(data);
	}
	{
		TextData data;
		data.oFill = interactingText;
		SetUIText(data);
	}
}

void UIButton::InteractEnd()
{
	m_onInteract();
}
} // namespace ME
