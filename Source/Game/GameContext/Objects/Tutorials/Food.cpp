#include "Food.h"
#include "Controller.h"

namespace ME
{
void Food::OnCreate()
{
	Instantiate(Primitive::Type::Circle);
	ShapeData data;
	data.oBorder = Fixed::One;
	SetShape(data);
}

void Food::OnHit(Collision::Info info)
{
	if (auto pPlayer = dynamic_cast<Controller*>(info.pInstigator))
	{
		LOG_D("[%s] Collided with %s, destroying [%s]", m_name.data(), pPlayer->Name().data(), m_name.data());
		Destroy();
	}
}

void Food::Tick(Time dt) 
{
	GameObject::Tick(dt);
}
} // namespace ME
