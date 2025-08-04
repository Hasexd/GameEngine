#include "ECS.h"
#include "UUID.h"

Entity ECS::CreateEntity()
{
	Entity entity = UUID::GenerateUUID();
	m_Entities.push_back(entity);
	return entity;
}