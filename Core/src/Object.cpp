#include "Object.h"


namespace Core
{
	Object::Object(ECS& ecs):
		m_ECS(ecs), m_UUID(ecs.CreateEntity()) 
	{
		AddComponent<Transform>();
	}
}
