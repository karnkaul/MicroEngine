#include "UIWidget.h"
#include "../../Worlds/GameWorld.h"
#include "Engine/Physics/ColliderData.h"

namespace ME
{
void UIWidget::OnCreate()
{
	m_hText = GameWorld::Active().NewObject<GameObject>(m_name + "_Text");
	if (auto pText = GameWorld::Active().FindObject<GameObject>(m_hText))
	{
		pText->Instantiate(Primitive::Type::Text);
		TextData data;
		data.oCharSize = 30;
		pText->SetText(data);
		pText->m_transform.SetParent(m_transform);
		pText->m_layer = m_layer + 1;
	}
	RegisterInput(
		[this](const Input::Frame& frame) -> bool {
			if (!m_flags[ToIdx(Flags::Enabled)])
			{
				return false;
			}
			Vector2 mousePos = frame.mouseInput.worldPosition;
			Rect2 bounds = Bounds();
			AABBData aabb(bounds.BottomLeft() + m_transform.WorldPosition(), bounds.TopRight() + m_transform.WorldPosition());
			bool bBlock = false;
			switch (m_state)
			{
			case State::Deselected:
			{
				if (aabb.IsPointInRect(mousePos))
				{
					m_state = State::Selected;
					Select();
				}
				break;
			}
			case State::Selected:
			{
				if (!aabb.IsPointInRect(mousePos))
				{
					m_state = State::Deselected;
					Deselect();
				}
				if (frame.IsPressed(KeyType::MOUSE_BTN_0) || frame.IsPressed(KeyCode::Enter))
				{
					m_state = State::Interacting;
					InteractBegin();
				}
				break;
			}
			case State::Interacting:
			{
				if (aabb.IsPointInRect(mousePos))
				{
					if (frame.IsReleased(KeyType::MOUSE_BTN_0) || frame.IsReleased(KeyCode::Enter))
					{
						m_state = State::Selected;
						InteractEnd();
						Select();
						bBlock = true;
					}
				}
				else
				{
					m_state = State::Deselected;
					Deselect();
				}
				break;
			}
			case State::UnInteractable:
			{
				break;
			}
			}
			return m_state == State::Interacting || bBlock;
		},
		true);
}

void UIWidget::OnDestroy()
{
	GameWorld::Active().DestroyObject(m_hText);
}

void UIWidget::SetEnabled(bool bEnabled)
{
	GameObject::SetEnabled(bEnabled);
	if (auto pText = GameWorld::Active().FindObject<GameObject>(m_hText))
	{
		pText->SetEnabled(bEnabled);
	}
}

void UIWidget::SetInteractable(bool bInteractable)
{
	if (bInteractable)
	{
		m_state = State::Deselected;
	}
	else
	{
		m_state = State::UnInteractable;
	}
}

UIWidget& UIWidget::SetUIText(const TextData& data)
{
	if (auto pText = GameWorld::Active().FindObject<GameObject>(m_hText))
	{
		pText->m_layer = m_layer + 1;
		pText->SetText(data);
		Vector2 textSize = pText->Bounds().Size();
		Vector2 shapeSize = Bounds().Size();
		if (textSize.x + textPad.x > shapeSize.x || textSize.y + textPad.y > shapeSize.y)
		{
			ShapeData data;
			data.oSize = shapeSize;
			if (textSize.x + textPad.x > shapeSize.x)
			{
				data.oSize->x = textSize.x + textPad.x;
			}
			if (textSize.y + textPad.y > shapeSize.y)
			{
				data.oSize->y = textSize.y + textPad.y;
			}
			SetShape(data);
		}
	}
	return *this;
}

UIWidget::OnInteract::Token UIWidget::Register(OnInteract::Callback callback)
{
	return m_onInteract.Register(callback);
}

UIWidget::State UIWidget::GetState() const
{
	return m_state;
}
} // namespace ME
