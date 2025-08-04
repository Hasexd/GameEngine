#include "Object.h"


Object::Object(ECS& ecs) :
	m_ECS(ecs), m_UUID(ecs.CreateEntity())
{}