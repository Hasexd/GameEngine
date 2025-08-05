#include "ECS.h"

namespace Core
{
	UUID ECS::CreateEntity()
	{
		UUID entity = Core::GenerateUUID();
		m_Entities.push_back(entity);
		return entity;
	}
}
