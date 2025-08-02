#include "ECS.h"
#include "UUID.h"

Entity ECS::CreateEntity()
{
	Entity entity = UUID::GenerateUUIDV4();
	m_Entities.push_back(entity);
	return entity;
}